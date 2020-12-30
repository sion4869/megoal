#include "potideko.h"
#include "Block.h"
//#include "Canon.h"
#include<vector>
#include<cmath>
using namespace std;
#define FOOTLENGTH 28.0f
#define FOOTHAEGIWAY 48.0f
#define FOOTHAEGIWAX 25.0f
#define SWITCHLENGTH 10.0f

extern std::vector<Object*> ObjectList;

Potideko::Potideko() {
	m_Centerbox = nullptr;
	m_Rightbox = nullptr;
	m_Leftbox = nullptr;
	FootRangeL[0] = nullptr;
	FootRangeL[1] = nullptr;
	FootRangeR[0] = nullptr;
	FootRangeR[1] = nullptr;
	SwitchRange = nullptr;
}

Potideko::~Potideko()
{
	if (m_Centerbox != nullptr)
		delete m_Centerbox;
	if (m_Rightbox != nullptr)
		delete m_Rightbox;
	if (m_Leftbox != nullptr)
		delete m_Leftbox;
	if (FootRangeL[0] != nullptr)
		delete FootRangeL[0];
	if (FootRangeL[1] != nullptr)
		delete FootRangeL[1];
	if (FootRangeR[0] != nullptr)
		delete FootRangeR[0];
	if (FootRangeR[1] != nullptr)
		delete FootRangeR[1];
	if (SwitchRange != nullptr)
		delete SwitchRange;
}

bool Potideko::Init(XMFLOAT2 origin) {
	bool sts;
	m_state = NEUTRAL;
	m_footPos = 0.0f;
	m_Math = POTIDEKOMATH;
	//�����ʒu�ݒ�
	SetPos(origin);
	SetPrePos(origin);
	FootRangeL[0] = new Segment;
	FootRangeL[1] = new Segment;
	FootRangeR[0] = new Segment;
	FootRangeR[1] = new Segment;
	SwitchRange = new Segment;

	FootRangeL[0]->SetSeg(origin.x - 23, origin.y, origin.x - 23, origin.y + 70);
	FootRangeL[1]->SetSeg(origin.x - 23, origin.y, origin.x - 23, origin.y + 70);
	FootRangeR[0]->SetSeg(origin.x + 23, origin.y, origin.x + 23, origin.y + 70);
	FootRangeR[1]->SetSeg(origin.x + 23, origin.y, origin.x + 23, origin.y + 70);
	SwitchRange->SetSeg(origin.x, origin.y, origin.x, origin.y - 10);

	//�q�b�g�{�b�N�X����
	m_Centerbox = new Hitbox;
	m_Rightbox = new Hitbox;
	m_Leftbox = new Hitbox;
	m_Centerbox->Init(this, hboxpos);
	m_Rightbox->Init(this, migipos);
	m_Leftbox->Init(this, hidaripos);

	//�O���[�o�������蔻�胊�X�g�ɓo�^
	//AddHitboxList(1);

	//uv���W
	XMFLOAT2 uv[4] = {
		{0,0},
		{1,0},
		{0,1},
		{1,1},
	};

	//���̃��[�J�����W
	XMFLOAT2 headpos[4] = {
		{-50,-50},
		{50,-50},
		{-50,50},
		{50,50},
	};

	//��̃��[�J�����W
	XMFLOAT2 facepos[4] = {
		{-25,0},
		{25,0},
		{-25,50},
		{25,50},
	};

	//���̃��[�J�����W
	XMFLOAT2 footpos[4] = {
		{-50,FOOTHAEGIWAY},
		{50,FOOTHAEGIWAY},
		{-50,FOOTHAEGIWAY + FOOTLENGTH},
		{50,FOOTHAEGIWAY + FOOTLENGTH},
	};

	//�e�N�X�`��������
	m_Head.Init(headpos, XMFLOAT4(1, 1, 1, 1), uv, 0.0f);
	m_Face.Init(facepos, XMFLOAT4(1, 1, 1, 1), uv, 0.0f);
	m_Foot.Init(footpos, XMFLOAT4(1, 1, 1, 1), uv, 0.0f);

	//�f�o�C�X�R���e�L�X�g�擾
	ID3D11Device* device = CDirectXGraphics::GetInstance()->GetDXDevice();
	ID3D11DeviceContext* devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();
	//�摜�ǂݍ���
	sts = CreateSRVfromFile("assets/doutai.png", device, devicecontext, &ResHead, &SrvHead);
	if (!sts)
	{
		MessageBox(nullptr, "CreateSRVfromfile �G���[", "error!!", MB_OK);
		return false;
	}
	sts = CreateSRVfromFile("assets/face1.png", device, devicecontext, &ResFace, &SrvFace);
	if (!sts)
	{
		MessageBox(nullptr, "CreateSRVfromfile �G���[", "error!!", MB_OK);
		return false;
	}
	sts = CreateSRVfromFile("assets/foot.png", device, devicecontext, &ResFoot, &SrvFoot);
	if (!sts)
	{
		MessageBox(nullptr, "CreateSRVfromfile �G���[", "error!!", MB_OK);
		return false;
	}
	return 1;

}

void Potideko::Update() {
	switch (m_state)
	{
	case NEUTRAL: {

		//�ʒu�L��
		m_PrePos = m_Pos;

		//�d��
		m_Speed.y += GRAVITY;

		if (CheckSwitch())
		{
			m_Pos.y = 0;
			m_PrePos.y = 0;
		}
		//����
		//��]
		//�^�[�Q�b�g�A���O�������߂āA�����Ɍ������ĉ�]���Ă�������
		if (GetKeyboardTrigger(DIK_RIGHT))
		{
			Rotate(0);
		}
		if (GetKeyboardTrigger(DIK_LEFT))
		{
			Rotate(1);
		}

		//�ړ�
		//�����₵�Ĉړ�
		if (FootSpring())
		{
			if (GetKeyboardTrigger(DIK_W))
			{
				m_Speed.y = -30;
			}
		}

		if (GetKeyboardPress(DIK_A))
		{
			m_Speed.x = -7;
		}
		if (GetKeyboardPress(DIK_D))
		{
			m_Speed.x = 7;
		}

		if (m_Speed.x > 0)
		{
			m_Speed.x -= 1;
			if (m_Speed.x < 0)m_Speed.x = 0.0f;
		}
		else if (m_Speed.x < 0)
		{
			m_Speed.x += 1;
			if (m_Speed.x > 0)m_Speed.x = 0.0f;

		}

		m_Head.SetRotation((float)m_Angle);
		m_Face.SetRotation((float)m_Angle);


		//�ʒu�ړ�
		m_Pos.x += m_Speed.x;
		m_Pos.y += m_Speed.y;

		break; }
	case TURN: {
		if (m_TargetAngle > m_Angle)
		{
			m_Angle += 15.0f;
		}
		else if (m_TargetAngle < m_Angle)
		{
			m_Angle -= 15.0f;
		}

		m_Head.SetRotation((float)m_Angle);
		m_Face.SetRotation((float)m_Angle);
		if (m_TargetAngle == m_Angle)
		{
			if (m_Angle == 180.0f)
			{
				m_state = SAKASA;
			}
			else
				m_state = NEUTRAL;
		}
		break; }
	case SAKASA: {
		//�ʒu�L��
		m_PrePos = m_Pos;

		//�d��
		m_Speed.y += 1.5f;

		//�X�C�b�`����
		if (CheckSwitch())
		{
			m_Pos.y = 0;
			m_PrePos.y = 0;
		}

		//�҂��҂��ړ�
		EarSpring();
		//����
		//��]
		//�^�[�Q�b�g�A���O�������߂āA�����Ɍ������ĉ�]���Ă�������
		if (GetKeyboardTrigger(DIK_RIGHT))
		{
			Rotate(0);

		}
		if (GetKeyboardTrigger(DIK_LEFT))
		{

			Rotate(1);
		}


		m_Head.SetRotation((float)m_Angle);
		m_Face.SetRotation((float)m_Angle);


		//�ʒu�ړ�
		m_Pos.x += m_Speed.x;
		m_Pos.y += m_Speed.y;
		break; }
	}
	//if (m_Pos.y > 500) { m_Pos.y = 500; SetSpeed(XMFLOAT2(m_Speed.x, 0)); }
}

bool Potideko::isGrounded(float* out)
{

	return false;
}

void Potideko::Draw() {
	ID3D11DeviceContext* devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// ��`�`��
	m_Head.SetPosition(m_Pos.x, m_Pos.y, 0);
	m_Face.SetPosition(m_Pos.x, m_Pos.y, 0);
	m_Foot.SetPosition(m_Pos.x, m_Pos.y + m_footPos, 0);
	if ((int)m_TargetAngle % 360 != 180)
	{
		devcontext->PSSetShaderResources(0, 1, SrvFoot.GetAddressOf());
		m_Foot.Draw();
	}
	devcontext->PSSetShaderResources(0, 1, SrvHead.GetAddressOf());
	m_Head.Draw();
	devcontext->PSSetShaderResources(0, 1, SrvFace.GetAddressOf());
	m_Face.Draw();
}

void Potideko::Rotate(bool dir)
{
	m_Centerbox->Rotate90(dir);
	m_Rightbox->Rotate90(dir);
	m_Leftbox->Rotate90(dir);

	//�X�e�[�W���񂷏���
	bool isOverrap = 0;
	bool rotateArea = 1;//0=�E�@1=��
	if (m_Pos.x < CENTERX)
		rotateArea = 0;
	for (auto o : ObjectList)
	{
		if (o != this && (typeid(*o) == typeid(Block)))
		{
			//�����Ȃ��u���b�N�̎�
			Block* pb = (Block*)o;
			if (pb->GetUseGravity() == false)
			{
				if ((!rotateArea && CENTERX > o->GetPos().x) ||
					(rotateArea && CENTERX <= o->GetPos().x))
				{
					if (pb->GetPos().y < STAGEBOTTOM)
					{
						pb->Rotate(dir);

						if (!isOverrap)
							for (auto selfbox : this->m_HitboxList)
								for (auto obox : o->m_HitboxList)
								{
									if (check_Overrap(*selfbox, *obox))
										isOverrap = 1;
								}
					}
				}
			}
			else//�����u���b�N�̎�
			{

				bool right = 0;
				bool left = 0;

				for (auto obox : o->m_HitboxList)
				{
					for (int i = 0; i < 4; i++)
					{
						if (obox->localpos[i].x + o->GetPos().x >= CENTERX)
							right = 1;
						else
							left = 1;
					}
				}
				XMFLOAT2 fix = XMFLOAT2(0.0f, 0.0f);

				

				//�񂳂񎞉񂹂Ȃ��Ƃ��͔��Α��̂ق��ɒ��S�_��u��
				if ((right==1&&left==1) || ((rotateArea != right)))
				{
					if (rotateArea)
					{
						fix.x = o->GetPos().x-AREAHALF;
						fix.y = o->GetPos().y-AREAHALF;
						o->SetPos(XMFLOAT2(AREAHALF, AREAHALF));
						o->SetPrePos(XMFLOAT2(AREAHALF, AREAHALF));
						for (auto obox : o->m_HitboxList)
						{
							obox->Move(fix);
						}
						float tmp;
						if (pb->GetTargetAngle() == 270.0f)
						{
							tmp = fix.x;
							fix.x = -fix.y;
							fix.y = tmp;
						}
						else if (pb->GetTargetAngle() == 180.0f)
						{

							fix.x = -fix.x;
							fix.y = -fix.y;
						}
						else if (pb->GetTargetAngle() == 90.0f)
						{
							tmp = fix.x;
							fix.x = fix.y;
							fix.y = -tmp;
						}
						pb->FixTexVertex(fix);

					}
					else
					{
						fix.x = o->GetPos().x - (CENTERX + AREAHALF);
						fix.y = o->GetPos().y- AREAHALF ;
						o->SetPos(XMFLOAT2(CENTERX + AREAHALF, AREAHALF));
						o->SetPrePos(XMFLOAT2(CENTERX + AREAHALF, AREAHALF));
						for (auto obox : o->m_HitboxList)
						{
							obox->Move(fix);
						}
						float tmp;
						if (pb->GetTargetAngle() == 270.0f)
						{
							tmp = fix.x;
							fix.x = -fix.y;
							fix.y = tmp;
						}
						else if (pb->GetTargetAngle() == 180.0f)
						{

							fix.x = -fix.x;
							fix.y = -fix.y;
						}
						else if (pb->GetTargetAngle() == 90.0f)
						{
							tmp = fix.x;
							fix.x = fix.y;
							fix.y = -tmp;
						}
						pb->FixTexVertex(fix);

					}
					if (right == 1 && left == 1)
					isOverrap = 1;
				}
				else//�񂷂Ƃ�
				{
					if (!rotateArea)
					{
						fix.x = o->GetPos().x - AREAHALF;
						fix.y = o->GetPos().y - AREAHALF;
						o->SetPos(XMFLOAT2(AREAHALF, AREAHALF));
						o->SetPrePos(XMFLOAT2(AREAHALF, AREAHALF));
						for (auto obox : o->m_HitboxList)
						{
							obox->Move(fix);
						}
						float tmp;
						if (pb->GetTargetAngle() == 270.0f)
						{
							tmp = fix.x;
							fix.x = -fix.y;
							fix.y = tmp;
						}
						else if (pb->GetTargetAngle() == 180.0f)
						{

							fix.x = -fix.x;
							fix.y = -fix.y;
						}
						else if (pb->GetTargetAngle() == 90.0f)
						{
							tmp = fix.x;
							fix.x = fix.y;
							fix.y = -tmp;
						}
						pb->FixTexVertex(fix);
					}
					else
					{
						fix.x = o->GetPos().x - (CENTERX + AREAHALF);
						fix.y = o->GetPos().y - AREAHALF;

						o->SetPos(XMFLOAT2(CENTERX + AREAHALF, AREAHALF));
						o->SetPrePos(XMFLOAT2(CENTERX + AREAHALF, AREAHALF));
						for (auto obox : o->m_HitboxList)
						{
							obox->Move(fix);
						}
						float tmp;
						if (pb->GetTargetAngle() == 270.0f)
						{
							tmp = fix.x;
							fix.x = -fix.y;
							fix.y = tmp;
						}
						else if (pb->GetTargetAngle() == 180.0f)
						{

							fix.x = -fix.x;
							fix.y = -fix.y;
						}
						else if (pb->GetTargetAngle() == 90.0f)
						{
							tmp = fix.x;
							fix.x = fix.y;
							fix.y = -tmp;
						}
						pb->FixTexVertex(fix);

					}
					pb->Rotate(dir);
					if (!isOverrap)
						for (auto selfbox : this->m_HitboxList)
							for (auto obox : o->m_HitboxList)
							{
								if (check_Overrap(*selfbox, *obox))
									isOverrap = 1;
							}
				}
			}
		}
	}
	//�d�Ȃ��Ă���߂��B
	if (isOverrap)
	{
		m_Centerbox->Rotate90(!dir);
		m_Rightbox->Rotate90(!dir);
		m_Leftbox->Rotate90(!dir);
		for (auto o : ObjectList)
		{
			if (o != this && (typeid(*o) == typeid(Block)))//���g����Ȃ��ĂȂ��ău���b�N�Ȃ�
			{
				Block* pb = (Block*)o;
				if (pb->GetUseGravity() == false)
				{
					if ((!rotateArea && CENTERX > o->GetPos().x) ||
						(rotateArea && CENTERX <= o->GetPos().x))
					{
						if (pb->GetPos().y < STAGEBOTTOM)
							pb->Rotate(!dir);
					}
				}
				else
				{
					if ((!rotateArea&&pb->GetPos().x == AREAHALF) || (rotateArea&&pb->GetPos().x != AREAHALF))
					{
						pb->Rotate(!dir);
					}
				}
			}
		}
	}
	else
	{
		//�v���C���[�̃e�N�X�`������]
		if (dir)
		{
			m_TargetAngle = (m_TargetAngle - 90.0f);
			if (m_TargetAngle < 0.0f)
			{
				m_Angle += 360.0f;
				m_TargetAngle += 360.0f;
			}
			m_state = TURN;

		}
		else
		{
			m_TargetAngle = (m_TargetAngle + 90.0f);
			if (m_TargetAngle >= 360.0f)
			{
				m_Angle -= 360.0f;
				m_TargetAngle -= 360.0f;
			}
			m_state = TURN;
		}
	}
}

bool Potideko::FootSpring()
{

	//���̔���//���̒���
	FootRangeL[0]->SetSeg(m_Pos.x - FOOTHAEGIWAX, m_Pos.y + FOOTHAEGIWAY, m_Pos.x - FOOTHAEGIWAX, m_Pos.y + FOOTHAEGIWAY + FOOTLENGTH);
	FootRangeL[1]->SetSeg(m_Pos.x - FOOTHAEGIWAX * 2, m_Pos.y + FOOTHAEGIWAY, m_Pos.x - FOOTHAEGIWAX * 2, m_Pos.y + FOOTHAEGIWAY + FOOTLENGTH);
	FootRangeR[0]->SetSeg(m_Pos.x + FOOTHAEGIWAX, m_Pos.y + FOOTHAEGIWAY, m_Pos.x + FOOTHAEGIWAX, m_Pos.y + FOOTHAEGIWAY + FOOTLENGTH);
	FootRangeR[1]->SetSeg(m_Pos.x + FOOTHAEGIWAX * 2, m_Pos.y + FOOTHAEGIWAY, m_Pos.x + FOOTHAEGIWAX * 2, m_Pos.y + FOOTHAEGIWAY + FOOTLENGTH);
	float distance = 2.0f;
	float sidedistance = 2.0f;

	for (auto o : ObjectList)
	{
		if (o != this)
		{
			for (auto obox : o->m_HitboxList)
			{


				float tmp;
				tmp = RaytoboxCollision(*FootRangeL[0], *obox);//�������n�ʂɐڂ��Ă��邩�ǂ���
				if (distance > tmp)
				{
					distance = tmp;
				}
				tmp = RaytoboxCollision(*FootRangeL[1], *obox);//�������n�ʂɐڂ��Ă��邩�ǂ���
				if (distance > tmp)
				{
					sidedistance = tmp;
				}
				tmp = RaytoboxCollision(*FootRangeR[0], *obox);//�E�����n�ʂɐڂ��Ă��邩�ǂ���
				if (distance > tmp)
				{
					distance = tmp;
				}
				tmp = RaytoboxCollision(*FootRangeR[1], *obox);//�E�����n�ʂɐڂ��Ă��邩�ǂ���
				if (distance > tmp)
				{
					sidedistance = tmp;
				}
			}
		}
	}

	if (distance <= 1.0f || sidedistance <= 1.0f)
	{
		if (distance <= 1.0f)
		{
			m_footPos = -FOOTLENGTH * (1.0f - distance);//�n�ʂƂ̋����ɂ���đ��̒�����ς���
			m_Speed.y -= 1.5f + (0.5f - distance);
			m_Speed.y *= 0.9f;
		}
		return true;
	}
	else
	{
		if (m_footPos < 0.0f)
			m_footPos += 1.0f;
	}


	return false;
}

bool Potideko::EarSpring()
{
	//���̔���//���̒���
	FootRangeL[0]->SetSeg(m_Pos.x - FOOTHAEGIWAX, m_Pos.y + FOOTHAEGIWAY, m_Pos.x - FOOTHAEGIWAX, m_Pos.y + FOOTHAEGIWAY + FOOTLENGTH);
	FootRangeL[1]->SetSeg(m_Pos.x - FOOTHAEGIWAX * 2, m_Pos.y + FOOTHAEGIWAY, m_Pos.x - FOOTHAEGIWAX * 2, m_Pos.y + FOOTHAEGIWAY + FOOTLENGTH);
	FootRangeR[0]->SetSeg(m_Pos.x + FOOTHAEGIWAX, m_Pos.y + FOOTHAEGIWAY, m_Pos.x + FOOTHAEGIWAX, m_Pos.y + FOOTHAEGIWAY + FOOTLENGTH);
	FootRangeR[1]->SetSeg(m_Pos.x + FOOTHAEGIWAX * 2, m_Pos.y + FOOTHAEGIWAY, m_Pos.x + FOOTHAEGIWAX * 2, m_Pos.y + FOOTHAEGIWAY + FOOTLENGTH);
	float distance = 2.0f;

	for (auto o : ObjectList)
	{
		if (o != this)
		{
			for (auto obox : o->m_HitboxList)
			{
				float tmp;

				tmp = RaytoboxCollision(*FootRangeL[0], *obox);//�������n�ʂɐڂ��Ă��邩�ǂ���
				if (distance > tmp)
				{
					distance = tmp;
				}
				tmp = RaytoboxCollision(*FootRangeL[1], *obox);//�������n�ʂɐڂ��Ă��邩�ǂ���
				if (distance > tmp)
				{
					distance = tmp;
				}
				tmp = RaytoboxCollision(*FootRangeR[0], *obox);//�E�����n�ʂɐڂ��Ă��邩�ǂ���
				if (distance > tmp)
				{
					distance = tmp;
				}
				tmp = RaytoboxCollision(*FootRangeR[1], *obox);//�E�����n�ʂɐڂ��Ă��邩�ǂ���
				if (distance > tmp)
				{
					distance = tmp;
				}
			}
		}
	}

	if (distance <= 0.5f)
	{
		//�ړ�
		if (GetKeyboardPress(DIK_W))
		{
			m_Speed.y = -30;
		}

		else if (GetKeyboardPress(DIK_A))
		{
			m_Speed.y = -10.0f;

		}
		else if (GetKeyboardPress(DIK_D))
		{
			m_Speed.y = -10.0f;

		}

		if (m_Speed.x > 0)
		{
			m_Speed.x -= 1;
			if (m_Speed.x < 0)m_Speed.x = 0.0f;
		}
		else if (m_Speed.x < 0)
		{
			m_Speed.x += 1;
			if (m_Speed.x > 0)m_Speed.x = 0.0f;

		}

		return true;
	}
	else
	{
		if (GetKeyboardPress(DIK_A))
		{
			m_Speed.x = -7;
		}
		if (GetKeyboardPress(DIK_D))
		{
			m_Speed.x = 7;
		}

		if (m_Speed.x > 0)
		{
			m_Speed.x -= 1;
			if (m_Speed.x < 0)m_Speed.x = 0.0f;
		}
		else if (m_Speed.x < 0)
		{
			m_Speed.x += 1;
			if (m_Speed.x > 0)m_Speed.x = 0.0f;

		}

	}


	return false;
}

bool Potideko::CheckSwitch()
{
	//�X�C�b�`�̔���
	if (m_TargetAngle == 0.0f)
		SwitchRange->SetSeg(m_Pos.x, m_Pos.y, m_Pos.x, m_Pos.y - SWITCHLENGTH);
	if (m_TargetAngle == 90.0f)
		SwitchRange->SetSeg(m_Pos.x, m_Pos.y, m_Pos.x + SWITCHLENGTH, m_Pos.y);
	if (m_TargetAngle == 180.0f)
		SwitchRange->SetSeg(m_Pos.x, m_Pos.y, m_Pos.x, m_Pos.y + SWITCHLENGTH);
	if (m_TargetAngle == 270.0f)
		SwitchRange->SetSeg(m_Pos.x, m_Pos.y, m_Pos.x - SWITCHLENGTH, m_Pos.y);

	for (auto o : ObjectList)
	{
		if (o != this)
		{
			for (auto obox : o->m_HitboxList)
			{
				if (1.0f > RaytoboxCollision(*SwitchRange, *obox))
				{
					return true;
				}
			}
		}
	}
	return false;
}
