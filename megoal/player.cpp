#include		<wrl/client.h>
#include		"Shader.h"
#include		"CDirectxGraphics.h"
#include		"quad.h"
#include		"game.h"
#include		"input.h"

using Microsoft::WRL::ComPtr;

// �X�s�[�h
const float SPEED = 2.0f;

// ��
const unsigned int g_width = 100;
const unsigned int g_height = 100;


static XMFLOAT2 g_pos = { SCREEN_X / 2.0f,SCREEN_Y / 2.0f };		// �����ʒu


// �l�p�`�N���X
static Quad	g_quad;

static ComPtr<ID3D11ShaderResourceView> g_srv;
static ComPtr < ID3D11Resource> g_res;

// ��ԗ񋓃N���X
enum class PLAYERSTATUS {
	LIVE,						// �����Ă�
	DEAD,						// ����
	HIT							// ��e��
};

// �v���C�����
static PLAYERSTATUS	g_sts = PLAYERSTATUS::LIVE;

// �����N���X
enum class DIRECTION {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

// �v���C������
static DIRECTION g_direction=DIRECTION::DOWN;

// �A�j���[�V�����p�t�u�e�[�u��
static XMFLOAT2 g_uv[12][4] =
{
	{
		{0.0f,			0.0f},						// ���ʂO		0-2
		{1.0 / 3.0f,	0.0f},
		{0.0f,			1.0f / 4.0f},
		{1.0f / 3.0f,	1.0f / 4.0f},
	},
	{
		{1.0 / 3.0f,	0.0f},						// ���ʂP
		{2.0 / 3.0f,	0.0f},
		{1.0f / 3.0f,	1.0f / 4.0f},
		{2.0f / 3.0f,	1.0f / 4.0f}
	},
	{
		{2.0 / 3.0f,	0.0f},						// ���ʂQ
		{3.0 / 3.0f,	0.0f},
		{2.0f / 3.0f,	1.0f / 4.0f},
		{3.0f / 3.0f,	1.0f / 4.0f}
	},

	{
		{0.0f,			1.0f / 4.0f},				// ���O				3-5
		{1.0 / 3.0f,	1.0f / 4.0f},
		{0.0f,			2.0f / 4.0f},
		{1.0f / 3.0f,	2.0f / 4.0f},
	},
	{
		{1.0 / 3.0f,	1.0f / 4.0f},				// ���P
		{2.0 / 3.0f,	1.0f / 4.0f},
		{1.0f / 3.0f,	2.0f / 4.0f},
		{2.0f / 3.0f,	2.0f / 4.0f}
	},
	{
		{2.0 / 3.0f,	1.0f / 4.0f},				// ���Q
		{3.0 / 3.0f,	1.0f / 4.0f},
		{2.0f / 3.0f,	2.0f / 4.0f},
		{3.0f / 3.0f,	2.0f / 4.0f}
	},
	{
		{0.0f,			2.0f / 4.0f},				// �E�O				6-8
		{1.0 / 3.0f,	2.0f / 4.0f},
		{0.0f,			3.0f / 4.0f},
		{1.0f / 3.0f,	3.0f / 4.0f},
	},
	{
		{1.0 / 3.0f,	2.0f / 4.0f},				// �E�P
		{2.0 / 3.0f,	2.0f / 4.0f},
		{1.0f / 3.0f,	3.0f / 4.0f},
		{2.0f / 3.0f,	3.0f / 4.0f}
	},
	{
		{2.0 / 3.0f,	2.0f / 4.0f},				// �E�Q
		{3.0 / 3.0f,	2.0f / 4.0f},
		{2.0f / 3.0f,	3.0f / 4.0f},
		{3.0f / 3.0f,	3.0f / 4.0f}
	},

	{
		{0.0f,			3.0f / 4.0f},				// ���O				9-11
		{1.0 / 3.0f,	3.0f / 4.0f},
		{0.0f,			4.0f / 4.0f},
		{1.0f / 3.0f,	4.0f / 4.0f},
	},
	{
		{1.0 / 3.0f,	3.0f / 4.0f},				// ���P
		{2.0 / 3.0f,	3.0f / 4.0f},
		{1.0f / 3.0f,	4.0f / 4.0f},
		{2.0f / 3.0f,	4.0f / 4.0f}
	},
	{
		{2.0 / 3.0f,	3.0f / 4.0f},				// ���Q
		{3.0 / 3.0f,	3.0f / 4.0f},
		{2.0f / 3.0f,	4.0f / 4.0f},
		{3.0f / 3.0f,	4.0f / 4.0f}
	}
};

// �A�j���[�V�����C���f�b�N�X
static int g_idx = 0;

bool InitPlayer() {

	bool sts;

	g_quad.Init(g_width, g_height, XMFLOAT4(1, 1, 1, 1), g_uv[0], 0.0f);

	ID3D11Device* device = CDirectXGraphics::GetInstance()->GetDXDevice();
	ID3D11DeviceContext* devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	sts = CreateSRVfromFile("assets/dora01.png", device, devicecontext, &g_res, &g_srv);
	if (!sts) {
		MessageBox(nullptr, "CreateSRVfromfile �G���[", "error!!", MB_OK);
		return false;
	}

}

void UpdatePlayer() {

	if (GetKeyboardPress(DIK_UP)) {
		g_direction = DIRECTION::UP;
		g_pos.y--;
	}

	if (GetKeyboardPress(DIK_DOWN)) {
		g_direction = DIRECTION::DOWN;
		g_pos.y++;
	}

	if (GetKeyboardPress(DIK_LEFT)) {
		g_direction = DIRECTION::LEFT;
		g_pos.x--;
	}

	if (GetKeyboardPress(DIK_RIGHT)) {
		g_direction = DIRECTION::RIGHT;
		g_pos.x++;
	}

}

void DrawPlayer() {

	unsigned int startidx=0;				// �����ɂ��A�j���[�V�����J�n�C���f�b�N�X
	unsigned int idx = 0;

	// �����ŃA�j���[�V������ς���
	switch (g_direction) {
	case DIRECTION::LEFT:
		startidx = 3;
		break;
	case DIRECTION::RIGHT:
		startidx = 6;
		break;
	case DIRECTION::UP:
		startidx = 9;
		break;
	case DIRECTION::DOWN:
		startidx = 0;
		break;
	}

	// �C���f�b�N�X��i�߂�
	idx = startidx + g_idx;

	//  UV���W�ύX
	g_quad.updateVertex(g_width, g_height, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), g_uv[idx]);
	g_quad.updateVbuffer();

	static int time = 0;
	time++;
	if (time % 5 == 0) {
		g_idx++;
		g_idx = g_idx % 3;
	}

	ID3D11DeviceContext* devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// ��`�`��
	devcontext->PSSetShaderResources(0, 1, g_srv.GetAddressOf());
	g_quad.SetPosition(g_pos.x, g_pos.y, 0);
	g_quad.Draw();

}

void ExitPlayer() {
	g_quad.UnInit();
}