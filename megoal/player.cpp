#include		<wrl/client.h>
#include		"Shader.h"
#include		"CDirectxGraphics.h"
#include		"quad.h"
#include		"game.h"
#include		"input.h"

using Microsoft::WRL::ComPtr;

// スピード
const float SPEED = 2.0f;

// 幅
const unsigned int g_width = 100;
const unsigned int g_height = 100;


static XMFLOAT2 g_pos = { SCREEN_X / 2.0f,SCREEN_Y / 2.0f };		// 初期位置


// 四角形クラス
static Quad	g_quad;

static ComPtr<ID3D11ShaderResourceView> g_srv;
static ComPtr < ID3D11Resource> g_res;

// 状態列挙クラス
enum class PLAYERSTATUS {
	LIVE,						// 生きてる
	DEAD,						// 死んだ
	HIT							// 被弾中
};

// プレイヤ状態
static PLAYERSTATUS	g_sts = PLAYERSTATUS::LIVE;

// 方向クラス
enum class DIRECTION {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

// プレイヤ方向
static DIRECTION g_direction=DIRECTION::DOWN;

// アニメーション用ＵＶテーブル
static XMFLOAT2 g_uv[12][4] =
{
	{
		{0.0f,			0.0f},						// 正面０		0-2
		{1.0 / 3.0f,	0.0f},
		{0.0f,			1.0f / 4.0f},
		{1.0f / 3.0f,	1.0f / 4.0f},
	},
	{
		{1.0 / 3.0f,	0.0f},						// 正面１
		{2.0 / 3.0f,	0.0f},
		{1.0f / 3.0f,	1.0f / 4.0f},
		{2.0f / 3.0f,	1.0f / 4.0f}
	},
	{
		{2.0 / 3.0f,	0.0f},						// 正面２
		{3.0 / 3.0f,	0.0f},
		{2.0f / 3.0f,	1.0f / 4.0f},
		{3.0f / 3.0f,	1.0f / 4.0f}
	},

	{
		{0.0f,			1.0f / 4.0f},				// 左０				3-5
		{1.0 / 3.0f,	1.0f / 4.0f},
		{0.0f,			2.0f / 4.0f},
		{1.0f / 3.0f,	2.0f / 4.0f},
	},
	{
		{1.0 / 3.0f,	1.0f / 4.0f},				// 左１
		{2.0 / 3.0f,	1.0f / 4.0f},
		{1.0f / 3.0f,	2.0f / 4.0f},
		{2.0f / 3.0f,	2.0f / 4.0f}
	},
	{
		{2.0 / 3.0f,	1.0f / 4.0f},				// 左２
		{3.0 / 3.0f,	1.0f / 4.0f},
		{2.0f / 3.0f,	2.0f / 4.0f},
		{3.0f / 3.0f,	2.0f / 4.0f}
	},
	{
		{0.0f,			2.0f / 4.0f},				// 右０				6-8
		{1.0 / 3.0f,	2.0f / 4.0f},
		{0.0f,			3.0f / 4.0f},
		{1.0f / 3.0f,	3.0f / 4.0f},
	},
	{
		{1.0 / 3.0f,	2.0f / 4.0f},				// 右１
		{2.0 / 3.0f,	2.0f / 4.0f},
		{1.0f / 3.0f,	3.0f / 4.0f},
		{2.0f / 3.0f,	3.0f / 4.0f}
	},
	{
		{2.0 / 3.0f,	2.0f / 4.0f},				// 右２
		{3.0 / 3.0f,	2.0f / 4.0f},
		{2.0f / 3.0f,	3.0f / 4.0f},
		{3.0f / 3.0f,	3.0f / 4.0f}
	},

	{
		{0.0f,			3.0f / 4.0f},				// 後ろ０				9-11
		{1.0 / 3.0f,	3.0f / 4.0f},
		{0.0f,			4.0f / 4.0f},
		{1.0f / 3.0f,	4.0f / 4.0f},
	},
	{
		{1.0 / 3.0f,	3.0f / 4.0f},				// 後ろ１
		{2.0 / 3.0f,	3.0f / 4.0f},
		{1.0f / 3.0f,	4.0f / 4.0f},
		{2.0f / 3.0f,	4.0f / 4.0f}
	},
	{
		{2.0 / 3.0f,	3.0f / 4.0f},				// 後ろ２
		{3.0 / 3.0f,	3.0f / 4.0f},
		{2.0f / 3.0f,	4.0f / 4.0f},
		{3.0f / 3.0f,	4.0f / 4.0f}
	}
};

// アニメーションインデックス
static int g_idx = 0;

bool InitPlayer() {

	bool sts;

	g_quad.Init(g_width, g_height, XMFLOAT4(1, 1, 1, 1), g_uv[0], 0.0f);

	ID3D11Device* device = CDirectXGraphics::GetInstance()->GetDXDevice();
	ID3D11DeviceContext* devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	sts = CreateSRVfromFile("assets/dora01.png", device, devicecontext, &g_res, &g_srv);
	if (!sts) {
		MessageBox(nullptr, "CreateSRVfromfile エラー", "error!!", MB_OK);
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

	unsigned int startidx=0;				// 方向によるアニメーション開始インデックス
	unsigned int idx = 0;

	// 方向でアニメーションを変える
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

	// インデックスを進める
	idx = startidx + g_idx;

	//  UV座標変更
	g_quad.updateVertex(g_width, g_height, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), g_uv[idx]);
	g_quad.updateVbuffer();

	static int time = 0;
	time++;
	if (time % 5 == 0) {
		g_idx++;
		g_idx = g_idx % 3;
	}

	ID3D11DeviceContext* devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// 矩形描画
	devcontext->PSSetShaderResources(0, 1, g_srv.GetAddressOf());
	g_quad.SetPosition(g_pos.x, g_pos.y, 0);
	g_quad.Draw();

}

void ExitPlayer() {
	g_quad.UnInit();
}