#include "Block.h"
#include <vector>

Block::Block() {
}

bool Block::Init(XMFLOAT2 origin, XMFLOAT2* vertex,XMFLOAT2 Speed) {
	bool sts;
	SetPos(origin);
	SetPrePos(origin);

	//�����蔻����W
	XMFLOAT2 hboxpos[4] = {
		{-5 * POTIDEKOSIZE,-5 * POTIDEKOSIZE},
		{5 * POTIDEKOSIZE,-5 * POTIDEKOSIZE},
		{-5 * POTIDEKOSIZE,5 * POTIDEKOSIZE},
		{5 * POTIDEKOSIZE,5 * POTIDEKOSIZE},
	};

	m_box = new Hitbox;
	m_box->Init(0, this, vertex);
	AddHitboxList(0);

	//uv���W
	XMFLOAT2 uv[4] = {
		{0,0},
		{1,0},
		{0,1},
		{1,1},
	};

	//�摜�̃��[�J�����W
	XMFLOAT2 texpos[4] = {
		{-5 * POTIDEKOSIZE,-5 * POTIDEKOSIZE},
		{5 * POTIDEKOSIZE,-5 * POTIDEKOSIZE},
		{-5 * POTIDEKOSIZE,5 * POTIDEKOSIZE},
		{5 * POTIDEKOSIZE,5 * POTIDEKOSIZE},
	};

	m_Texture.Init(vertex, XMFLOAT4(1, 1, 1, 1), uv, 0.0f);

	ID3D11Device* device = CDirectXGraphics::GetInstance()->GetDXDevice();
	ID3D11DeviceContext* devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	sts = CreateSRVfromFile("assets/tex1.bmp", device, devicecontext, &ResBlock, &SrvBlock);
	if (!sts)
	{
		MessageBox(nullptr, "CreateSRVfromfile �G���[", "error!!", MB_OK);
		return false;
	}

	sts = CreateSRVfromFile("assets/endswitch.png", device, devicecontext, &ResEnd, &SrvEnd);
	if (!sts)
	{
		MessageBox(nullptr, "CreateSRVfromfile �G���[", "error!!", MB_OK);
		return false;
	}
}

void Block::Update() {
	//�ʒu�L��
	m_PrePos = m_Pos;
	//�ʒu�ړ�
	//m_Speed.y = 2;
	m_Pos.x += m_Speed.x;
	m_Pos.y += m_Speed.y;

	//Speed.y += 2;

	if (m_Speed.x > 0)m_Speed.x -= 2;
	if (m_Speed.x < 0)m_Speed.x += 2;
}

void Block::Draw() {
	ID3D11DeviceContext* devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// ��`�`��
	m_Texture.SetPosition(m_Pos.x, m_Pos.y, 0);
	m_End.SetPosition(m_Pos.x, m_Pos.y, 0);
	devcontext->PSSetShaderResources(0, 1, SrvBlock.GetAddressOf());
	m_Texture.Draw();
	devcontext->PSSetShaderResources(0, 1, SrvEnd.GetAddressOf());
	m_End.Draw();
}
