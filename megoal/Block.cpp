#include "Block.h"
#include <vector>

Block::Block(){
}

bool Block::Init(XMFLOAT2 origin, XMFLOAT2* vertex, const char* texfile,float angle,XMFLOAT2 Speed) {
	m_state = NEUTRAL;
	useGravity = 0;
	m_Math = 0;
	Btype = NORMALBLOCK;
	bool sts;
	
	XMFLOAT2 fix=XMFLOAT2(0.0f,0.0f);

	//角度を合わせる
	m_Angle = angle;
	m_TargetAngle = angle;

	//位置によってどちらのエリアに属すかを判定する
	if (origin.y < STAGEBOTTOM)
	{
		if (origin.x < CENTERX)
		{
			fix.x = origin.x - AREAHALF;
			fix.y = origin.y - AREAHALF;
			origin.x = AREAHALF;
			origin.y = AREAHALF;

		}
		else
		{
			fix.x = origin.x - (CENTERX + AREAHALF);
			fix.y = origin.y - AREAHALF;
			origin.x = CENTERX+ AREAHALF;
			origin.y = AREAHALF;
		}
	}
	SetPos(origin);
	SetPrePos(origin);

	XMFLOAT2 fixedvertex[4];
	for (int i = 0; i < 4; i++)
	{
		fixedvertex[i].x=vertex[i].x + fix.x;
		fixedvertex[i].y=vertex[i].y + fix.y;
	}
	m_box = new Hitbox;
	m_box->Init(this, fixedvertex);




	//AddHitboxList(0);

	//uv座標
	XMFLOAT2 uv[4] = {
		{0,0},
		{1,0},
		{0,1},
		{1,1},
	};

	m_Texture.Init(fixedvertex, XMFLOAT4(1, 1, 1, 1), uv, 0.0f);
	//回転角を合わせる
	m_Texture.SetRotation((float)m_Angle);

	ID3D11Device* device = CDirectXGraphics::GetInstance()->GetDXDevice();
	ID3D11DeviceContext* devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	sts = CreateSRVfromFile(texfile, device, devicecontext, &ResBlock, &SrvBlock);
	if (!sts)
	{
		MessageBox(nullptr, "CreateSRVfromfile エラー", "error!!", MB_OK);
		return false;
	}

	//角度を合わせる
	for (int i = 0; i < (angle / 90.0f); i++)
	{
		m_box->Rotate90(0);
	}
}

void Block::Update() {
	switch (m_state)
	{
	case NEUTRAL:
	{
		//位置記憶
		m_PrePos = m_Pos;
		//移動
		if (GetKeyboardTrigger(DIK_LEFT))
		{
			/*if(m_Pos.y<1000)
			Rotate(1);*/
		}
		if (GetKeyboardTrigger(DIK_RIGHT))
		{
			/*if (m_Pos.y < 1000)
				Rotate(0);*/
		}
		if (GetKeyboardPress(DIK_J)&&m_Math==0)
		{
			m_Speed.x = -7;
		}
		if (GetKeyboardPress(DIK_L) && m_Math == 0)
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

		if (m_TargetAngle > m_Angle)
		{
			m_Angle += 15.0f;
		}
		else if (m_TargetAngle < m_Angle)
		{
			m_Angle -= 15.0f;
		}
		//重力を受ける時
		if (useGravity)
		{
			m_Speed.y += GRAVITY;
		}

		//キャノンの時
		if (Btype == CANONBLOCK)
		{

		}

		//位置移動
		m_Pos.x += m_Speed.x;
		m_Pos.y += m_Speed.y;
		break;
	}
	case TURN:
	{
		if (m_TargetAngle > m_Angle)
		{
			m_Angle += 15.0f;
		}
		else if (m_TargetAngle < m_Angle)
		{
			m_Angle -= 15.0f;
		}

		m_Texture.SetRotation((float)m_Angle);
		if (m_TargetAngle == m_Angle)
		{
				m_state = NEUTRAL;
		}
		break;
	}
	}
	m_fixMath = 0;
}

void Block::Draw() {
	ID3D11DeviceContext* devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// 矩形描画
	m_Texture.SetPosition(m_Pos.x, m_Pos.y, 0);
	devcontext->PSSetShaderResources(0, 1, SrvBlock.GetAddressOf());
	m_Texture.Draw();
}

void Block::Rotate(bool dir)
{
	if (dir == 0)
	{
		m_TargetAngle = (m_TargetAngle + 90.0f);
		if (m_TargetAngle >= 360.0f)
		{
			m_Angle -= 360.0f;
			m_TargetAngle -= 360.0f;
		}
		m_state = TURN;
	}
	else
	{
		m_TargetAngle = (m_TargetAngle - 90.0f);
		if (m_TargetAngle < 0.0f)
		{
			m_Angle += 360.0f;
			m_TargetAngle += 360.0f;
		}
		m_state = TURN;
	}
	m_box->Rotate90(dir);

}

void Block::SetUseGravity(bool g)
{
	useGravity = g;
}

bool Block::GetUseGravity()
{
	return useGravity;
}

void Block::FixTexVertex(XMFLOAT2 fix)
{
	m_Texture.MoveVertex(fix);
	//uv座標
	XMFLOAT2 uv[4] = {
		{0,0},
		{1,0},
		{0,1},
		{1,1},
	};
	m_Texture.updateVertex(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), uv, 0.0f);
	m_Texture.updateVbuffer();

}

void Block::SetType(int bt)
{
	Btype = bt;
}
