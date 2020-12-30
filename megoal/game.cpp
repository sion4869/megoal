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
#include "StageGenerator.h"
#include "Controller.h"
#define rep(n) for(int i =0;i<n;i++)
//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
//Potideko g_Potideko;

std::vector<Object*> ObjectList;

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
	
	StageInfo SI;
	SI.ObjInfo[0].objtype =GRAV;
	SI.ObjInfo[0].Angle = 0;
	SI.ObjInfo[0].Pos = { CENTERX - 100,STAGEBOTTOM-200.0f};
	SI.ObjInfo[1].objtype = STICK;
	SI.ObjInfo[1].Angle = 0;
	SI.ObjInfo[1].Pos = { CENTERX*2 - 100,STAGEBOTTOM - 50.0f };
	SI.ObjInfo[2].objtype = ITA;
	SI.ObjInfo[2].Angle = 0;
	SI.ObjInfo[2].Pos = { 500.0f,900.0f };
	SI.ObjInfo[3].objtype = ITA;
	SI.ObjInfo[3].Angle = 0;
	SI.ObjInfo[3].Pos = { 1400.0f,900.0f };
	SI.num = 4;
	StageGenerator::GetInstance()->GenStage(SI);
	
	/*g_block3.Init(XMFLOAT2(800, 350), epos, "assets/tex1.bmp");
	g_block2.Init(XMFLOAT2(500, 500), epos2, "assets/tex1.bmp");
	g_block1.Init(XMFLOAT2(400, 200), epos3, "assets/tex1.bmp");
*/
	return	true;
}

//==============================================================================
//!	@fn		GameInput
//!	@brief	ゲーム入力検知処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void Game::GameInput(GameEnum::Scene scene) {
	UpdateInput();

	//switch (scene) {
	//case GameEnum::Scene::TITLE: {
	//	//start押したとき
	//	if (control2->GetPadState(control2->PAD_START)) {
	//		gameScene = GameEnum::Scene::PLAY;
	//		//PlaySound(SOUND_LABEL_BGM000);
	//	}
	//	break; }


	//case GameEnum::Scene::PLAY: {  //firststage
	//	if (control2->GetPadState(control2->PAD_B)) {
	//		gameScene = GameEnum::Scene::END;

	//		//PlaySound(SOUND_LABEL_BGM000);
	//	}
	//	break; }
	//case GameEnum::Scene::END: {  //firststage


	//	cnt++;
	//	if (cnt <= 180) {
	//		control2->EnableVibration(1.0f, 1.0f);
	//	}
	//	else {
	//		control2->DisableVibration();
	//	}
	//	if (control2->GetPadState(control2->PAD_A)) {
	//		gameScene = GameEnum::Scene::TITLE;
	//		control2->DisableVibration();
	//		cnt = 0;
	//		/*control2->DisableVibration();*/
	//		//PlaySound(SOUND_LABEL_BGM000);
	//	}
	//	break; }
	//}
}

//==============================================================================
//!	@fn		GameUpdate
//!	@brief	ゲーム更新処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void Game::GameUpdate(GameEnum::Scene scene) {
	/*for (auto o : ObjectList)
	{
		o->Update();
	}*/
	//g_Potideko.Update();
	for (auto o : ObjectList)
	{
		o->Update();
	}
	

	/*UpdateInput();

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

	}*/

	Collision();
	//Collision();
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

	/*switch (gameScene)
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
*/


	for (auto o : ObjectList)
	{
		o->Draw();
	}
	//g_Potideko.Draw();

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

	UninitInput();

	// DirectX11 グラフィックス終了処理
	CDirectXGraphics::GetInstance()->Exit();
}

//******************************************************************************
//	End of file.
//******************************************************************************
