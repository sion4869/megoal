//*****************************************************************************
//!	@file	game.cpp
//!	@brief	
//!	@note	ゲーム処理
//!	@author	
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include <windows.h>
#include <stdio.h>
#include <wrl/client.h>

#include "CDirectxGraphics.h"
#include "DX11Settransform.h"
#include "2dsystem.h"
#include "quad.h"
#include "player.h"
#include "input.h"
#include "potideko.h"
#include "hitbox.h"
#include "Block.h"
#include "game.h"
#include "Sprite2D.h"
#include "Controller.h"




#define rep(n) for(int i =0;i<n;i++)
//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
Potideko g_Potideko;
Block g_block1;
Block g_block2;
Block g_block3;

Block g_end;

Sprite2D* g_title;
Sprite2D* g_end2;

Controller* control2;
int cnt = 0;

std::vector<Object*> ObjectList;
std::vector<std::vector<Hitbox*>> g_ActiveHitboxList;
std::vector<std::vector<Hitbox*>> g_PassiveHitboxList;

using Microsoft::WRL::ComPtr;
ComPtr<ID3D11ShaderResourceView> g_srv[GameEnum::Image::IMAGE_MAX]; //テクスチャをシェーダーで利用するために必要
ComPtr<ID3D11Resource> g_resource[GameEnum::Image::IMAGE_MAX];      //テクスチャオブジェクト


//==============================================================================
//!	@fn		GameInit
//!	@brief	ゲーム初期処理
//!	@param	インスタンス値
//!	@param	ウインドウハンドル値
//!	@param	ウインドウサイズ幅（補正済みの値）
//!	@param	ウインドウサイズ高さ（補正済みの値）
//!	@param　フルスクリーンフラグ　true ; フルスクリーン　false : ウインドウ
//!	@retval	true 成功　false 失敗
//==============================================================================
bool Game::GameInit(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen)
{
	// DirectX11 グラフィックス初期化
	bool sts = CDirectXGraphics::GetInstance()->Init(hwnd, width, height, fullscreen);
	if (!sts)
	{
		MessageBox(nullptr, "CDirectXGraphics Initエラー", "error!!", MB_OK);
		return false;
	}

	// 行列を渡す定数バッファを初期化
	DX11SetTransform::GetInstance()->Init();

	//　スクリーン座標に変換するための定数バッファを初期化
	Init2D();

	// アルファブレンディングを有効にする
	CDirectXGraphics::GetInstance()->TurnOnAlphaBlending();

	// DIRECTINPUT 初期化
	InitInput(hinst, hwnd);

	//当たり判定
	XMFLOAT2 ppos[4] = {
		{-5 * POTIDEKOSIZE,-5 * POTIDEKOSIZE},
		{5 * POTIDEKOSIZE,-5 * POTIDEKOSIZE},
		{-5 * POTIDEKOSIZE,5 * POTIDEKOSIZE},
		{5 * POTIDEKOSIZE,5 * POTIDEKOSIZE},
	};
	//当たり判定
	XMFLOAT2 epos[4] = {
		{-100,-100},
		{100,-100},
		{-200,100},
		{100,100},
	};

	XMFLOAT2 epos3[4] = {
		{-10,50},
		{10,50},
		{-30,100},
		{30,100},
	};

	//当たり判定
	XMFLOAT2 epos2[4] = {
		{-500,-50},
		{500,-50},
		{-500,50},
		{500,50},
	};


	g_Potideko.Init(XMFLOAT2(200, 200), ppos);
	g_block3.Init(XMFLOAT2(800, 350), epos);
	g_block2.Init(XMFLOAT2(500, 500), epos2);
	g_block1.Init(XMFLOAT2(400, 200), epos3);

	g_end.Init(XMFLOAT2(400, 200), epos3);

	g_title = new Sprite2D();
	HRESULT hr2 = g_title->Initialize();
	if (FAILED(hr2)) return false;

	g_title->LoadTexture("assets/TitleBack.png");
	if (FAILED(hr2)) return false;

	Sprite2D::CreateShader();
	if (FAILED(hr2)) return false;

	g_end2 = new Sprite2D();
	HRESULT hr3 = g_end2->Initialize();
	if (FAILED(hr3)) return false;

	g_end2->LoadTexture("assets/endswitch.png");
	if (FAILED(hr3)) return false;

	Sprite2D::CreateShader();
	if (FAILED(hr3)) return false;
	control2 = new Controller(1);
	return	true;
}

//==============================================================================
//!	@fn		GameInput
//!	@brief	ゲーム入力検知処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void Game::GameInput(GameEnum::Scene scene) {
	/*UpdateInput();*/
	/*control = new Controller(1);*/
	switch (scene) {
	case GameEnum::Scene::TITLE: {
		//start押したとき
		if (control2->GetPadState(control2->PAD_START)) {
			gameScene = GameEnum::Scene::PLAY;
			//PlaySound(SOUND_LABEL_BGM000);
		}
		break; }


	case GameEnum::Scene::PLAY: {  //firststage
		if (control2->GetPadState(control2->PAD_B)) {
			gameScene = GameEnum::Scene::END;
			
			//PlaySound(SOUND_LABEL_BGM000);
		}
		break; }
	case GameEnum::Scene::END: {  //firststage

		
		cnt++;
		if (cnt <= 180) {
			control2->EnableVibration(1.0f, 1.0f);
		}
		else {
			control2->DisableVibration();
		}
		if (control2->GetPadState(control2->PAD_A)) {
			gameScene = GameEnum::Scene::TITLE;
			control2->DisableVibration();
			cnt = 0;
			/*control2->DisableVibration();*/
			//PlaySound(SOUND_LABEL_BGM000);
		}
		break; }
	}
}

//==============================================================================
//!	@fn		GameUpdate
//!	@brief	ゲーム更新処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void Game::GameUpdate(GameEnum::Scene scene) {
	
	UpdateInput();

	switch (scene)
	{
	case GameEnum::Scene::TITLE: {
	
		break; }

	case GameEnum::Scene::PLAY: {
		g_Potideko.Update();
		g_block1.Update();
		g_block2.Update();
		g_block3.Update();

		Collition();
		break; }
	case GameEnum::Scene::END: {
		g_end.Update();
		break; }

	}
}

//==============================================================================
//!	@fn		GameRender
//!	@brief	ゲーム更新処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void Game::GameRender(GameEnum::Scene scene) {

	// ターゲットバッファクリア	
	float ClearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f }; //red,green,blue,alpha（ＲＧＢの各値を0.0～1.0で指定）

	// デバイスコンテキスト取得
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// レンダーターゲットビュー取得
	ID3D11RenderTargetView* rtv;
	rtv = CDirectXGraphics::GetInstance()->GetRenderTargetView();

	// ターゲットバッファクリア
	devcontext->ClearRenderTargetView(
		rtv,			// レンダーターゲットビュー
		ClearColor);	// クリアするカラー値

	// デプスステンシルビュー取得
	ID3D11DepthStencilView* dstv;
	dstv = CDirectXGraphics::GetInstance()->GetDepthStencilView();

	// Zバッファ、ステンシルバッファクリア
	devcontext->ClearDepthStencilView(
		dstv,			// デプスステンシルビュー
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,			// Ｚバッファを1.0でクリアする(0.0f～1.0f)
		0);				// ステンシルバッファを0でクリアする

	

	switch (gameScene)
	{
	case GameEnum::Scene::TITLE: {
		g_title->Draw2();
		break; }


	case GameEnum::Scene::PLAY: {
		g_Potideko.Draw();
		g_block1.Draw();
		g_block2.Draw();
		g_block3.Draw();
		break; }
	case GameEnum::Scene::END: {
		g_end2->Draw2();
		break; }

	}

	// 描画後処理
	IDXGISwapChain* swapchain;
	swapchain = CDirectXGraphics::GetInstance()->GetSwapChain();
	swapchain->Present(
		0,		// フレームの表示を垂直方向の空白と同期させる方法を指定する整数。
		0);		// スワップチェーン表示オプションを含む整数値。 
				// これらのオプションはDXGI_PRESENT定数で定義されています。
}

//==============================================================================
//!	@fn		GameMain
//!	@brief	ゲームループ処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void Game::GameMain()
{
	switch (gameScene) {
	case  GameEnum::Scene::TITLE:
		GameInput(gameScene); // 入力
		GameUpdate(gameScene);
		GameRender(gameScene);	 // 描画	
		break;

	case  GameEnum::Scene::PLAY:
		GameInput(gameScene); // 入力
		GameUpdate(gameScene);
		GameRender(gameScene);	// 描画	
		break;
	case  GameEnum::Scene::END:
		GameInput(gameScene); // 入力
		GameUpdate(gameScene);
		GameRender(gameScene);	// 描画	
		break;
	}
}

//==============================================================================
//!	@fn		GameExit
//!	@brief	ゲーム終了処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void Game::GameExit()
{
	Sprite2D::ReleaseShader();
	if (g_title != nullptr) {
		delete g_title;
		g_title = nullptr;
	}

	UninitInput();

	// DirectX11 グラフィックス終了処理
	CDirectXGraphics::GetInstance()->Exit();
}

//******************************************************************************
//	End of file.
//******************************************************************************
