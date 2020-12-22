#include "potideko.h"
#include "input.h"
#include<vector>
#include "Controller.h"

using namespace std;

Controller* control;
int old_key;
int new_key;

int old_key2;
int new_key2;


Potideko::Potideko() {
}
bool Potideko::Init(XMFLOAT2 origin, XMFLOAT2* vertex, XMFLOAT2 Speed) {
	bool sts;	
	
	//�q�b�g�{�b�N�X����
	m_Centerbox = new Hitbox;
	m_Rightbox = new Hitbox;
	m_Leftbox = new Hitbox;
	m_Centerbox->Init(1, this, hboxpos);
	m_Rightbox->Init(1, this, migipos);
	m_Leftbox->Init(1, this, hidaripos);

	//�O���[�o�������蔻�胊�X�g�ɓo�^
	AddHitboxList(1);

	//uv���W
	XMFLOAT2 uv[4] = {
		{0,0},
		{1,0},
		{0,1},
		{1,1},
	};

	//���̃��[�J�����W
	XMFLOAT2 headpos[4] = {
		{-5 * POTIDEKOSIZE,0},
		{5 * POTIDEKOSIZE,0},
		{-5 * POTIDEKOSIZE,5 * POTIDEKOSIZE},
		{5 * POTIDEKOSIZE,5 * POTIDEKOSIZE},
	};

	//���̃��[�J�����W
	XMFLOAT2 mimipos[4] = {
		{-5 * POTIDEKOSIZE,-5 * POTIDEKOSIZE},
		{5 * POTIDEKOSIZE,-5 * POTIDEKOSIZE},
		{-5 * POTIDEKOSIZE,0},
		{5 * POTIDEKOSIZE,0},
	};

	//�e�N�X�`��������
	m_Head.Init(headpos, XMFLOAT4(1, 1, 1, 1), uv, 0.0f);
	m_Ear.Init(mimipos, XMFLOAT4(1, 1, 1, 1), uv, 0.0f);

	//�f�o�C�X�R���e�L�X�g�擾
	ID3D11Device* device = CDirectXGraphics::GetInstance()->GetDXDevice();
	ID3D11DeviceContext* devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();
	//�摜�ǂݍ���
	sts = CreateSRVfromFile("assets/hontai.png", device, devicecontext, &ResHead, &SrvHead);
	if (!sts)
	{
		MessageBox(nullptr, "CreateSRVfromfile �G���[", "error!!", MB_OK);
		return false;
	}
	sts = CreateSRVfromFile("assets/mimi.png", device, devicecontext, &ResEar, &SrvEar);
	if (!sts)
	{
		MessageBox(nullptr, "CreateSRVfromfile �G���[", "error!!", MB_OK);
		return false;
	}
	//�����ʒu�ݒ�
	SetPos(XMFLOAT2(0, 0));

	control = new Controller(1);
}

void Potideko::Update() {
	//�ʒu�L��
	m_PrePos = m_Pos;
	//����
	//��]
	if (control->GetPadState(control->PAD_R_SHOULDER))
	{
		/*m_TargetAngle = (m_TargetAngle + 90.0f);
		Rotate(0);*/
		new_key = control->PAD_R_SHOULDER;
	}
	else
	{
		new_key = 0;
	}
	if ((new_key^old_key)&new_key)
	{
		m_TargetAngle = (m_TargetAngle + 90.0f);
		Rotate(0);
	}

	old_key = new_key;

	if (control->GetPadState(control->PAD_L_SHOULDER))
	{
	/*	m_TargetAngle = (m_TargetAngle - 90.0f);
		Rotate(1);*/
		new_key2 = control->PAD_L_SHOULDER;
	}
	else
	{
		new_key2 = 0;
	}
	if ((new_key2^old_key2)&new_key2)
	{
		m_TargetAngle = (m_TargetAngle - 90.0f);
		Rotate(1);
	}
	old_key2 = new_key2;

	//�ړ�
	if (control->GetPadState(control->PAD_A))
	{
		m_Speed.y = -30;
	}

	if (control->GetPadState(control->PAD_LEFT_L_STICK))
	{
		m_Speed.x = -7;
	}
	if (control->GetPadState(control->PAD_LEFT_R_STICK))
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



	if (GetKeyboardPress(DIK_T))
	{
		SetPos(XMFLOAT2(0, 0));
		SetPrePos(XMFLOAT2(0, 0));
	}


	if (m_TargetAngle > m_Angle)
	{
		m_Angle += 15.0f;
	}
	else if (m_TargetAngle < m_Angle)
	{
		m_Angle -= 15.0f;
	}


	m_Head.SetRotation((float)m_Angle);
	m_Ear.SetRotation((float)m_Angle);

	m_Speed.y += 1.5f;

	//�ʒu�ړ�
	m_Pos.x += m_Speed.x;
	m_Pos.y += m_Speed.y;



	//if (m_Pos.y > 500) { m_Pos.y = 500; SetSpeed(XMFLOAT2(m_Speed.x, 0)); }
}

void Potideko::Draw() {
	ID3D11DeviceContext* devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// ��`�`��
	m_Head.SetPosition(m_Pos.x, m_Pos.y, 0);
	m_Ear.SetPosition(m_Pos.x, m_Pos.y, 0);
	devcontext->PSSetShaderResources(0, 1, SrvHead.GetAddressOf());
	m_Head.Draw();
	devcontext->PSSetShaderResources(0, 1, SrvEar.GetAddressOf());
	m_Ear.Draw();
}

void Potideko::Rotate(bool dir)
{
	m_Centerbox->Rotate90(dir);
	m_Rightbox->Rotate90(dir);
	m_Leftbox->Rotate90(dir);
}
