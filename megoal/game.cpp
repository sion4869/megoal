//*****************************************************************************
//!	@file	game.cpp
//!	@brief	
//!	@note	�Q�[������
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
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
//Potideko g_Potideko;

std::vector<Object*> ObjectList;

//==============================================================================
//!	@fn		GameInit
//!	@brief	�Q�[����������
//!	@param	�C���X�^���X�l
//!	@param	�E�C���h�E�n���h���l
//!	@param	�E�C���h�E�T�C�Y���i�␳�ς݂̒l�j
//!	@param	�E�C���h�E�T�C�Y�����i�␳�ς݂̒l�j
//!	@param�@�t���X�N���[���t���O�@true ; �t���X�N���[���@false : �E�C���h�E
//!	@retval	true �����@false ���s
//==============================================================================
bool Game::GameInit(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen)
{
	// DirectX11 �O���t�B�b�N�X������
	bool sts = CDirectXGraphics::GetInstance()->Init(hwnd, width, height, fullscreen);
	if (!sts)
	{
		MessageBox(nullptr, "CDirectXGraphics Init�G���[", "error!!", MB_OK);
		return false;
	}

	// �s���n���萔�o�b�t�@��������
	DX11SetTransform::GetInstance()->Init();

	//�@�X�N���[�����W�ɕϊ����邽�߂̒萔�o�b�t�@��������
	Init2D();

	// �A���t�@�u�����f�B���O��L���ɂ���
	CDirectXGraphics::GetInstance()->TurnOnAlphaBlending();

	// DIRECTINPUT ������
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
//!	@brief	�Q�[�����͌��m����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void Game::GameInput(GameEnum::Scene scene) {
	UpdateInput();

	//switch (scene) {
	//case GameEnum::Scene::TITLE: {
	//	//start�������Ƃ�
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
//!	@brief	�Q�[���X�V����
//!	@param�@	
//!	@retval	�Ȃ�
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
//!	@brief	�Q�[���X�V����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void Game::GameRender(GameEnum::Scene scene) {

	// �^�[�Q�b�g�o�b�t�@�N���A	
	float ClearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f }; //red,green,blue,alpha�i�q�f�a�̊e�l��0.0�`1.0�Ŏw��j

	// �f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// �����_�[�^�[�Q�b�g�r���[�擾
	ID3D11RenderTargetView* rtv;
	rtv = CDirectXGraphics::GetInstance()->GetRenderTargetView();

	// �^�[�Q�b�g�o�b�t�@�N���A
	devcontext->ClearRenderTargetView(
		rtv,			// �����_�[�^�[�Q�b�g�r���[
		ClearColor);	// �N���A����J���[�l

	// �f�v�X�X�e���V���r���[�擾
	ID3D11DepthStencilView* dstv;
	dstv = CDirectXGraphics::GetInstance()->GetDepthStencilView();

	// Z�o�b�t�@�A�X�e���V���o�b�t�@�N���A
	devcontext->ClearDepthStencilView(
		dstv,			// �f�v�X�X�e���V���r���[
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,			// �y�o�b�t�@��1.0�ŃN���A����(0.0f�`1.0f)
		0);				// �X�e���V���o�b�t�@��0�ŃN���A����

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

	// �`��㏈��
	IDXGISwapChain* swapchain;
	swapchain = CDirectXGraphics::GetInstance()->GetSwapChain();
	swapchain->Present(
		0,		// �t���[���̕\���𐂒������̋󔒂Ɠ�����������@���w�肷�鐮���B
		0);		// �X���b�v�`�F�[���\���I�v�V�������܂ސ����l�B 
				// �����̃I�v�V������DXGI_PRESENT�萔�Œ�`����Ă��܂��B
}

//==============================================================================
//!	@fn		GameMain
//!	@brief	�Q�[�����[�v����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void Game::GameMain()
{
	switch (gameScene) {
	case  GameEnum::Scene::TITLE:
		GameInput(gameScene); // ����
		GameUpdate(gameScene);
		GameRender(gameScene);	 // �`��	
		break;

	case  GameEnum::Scene::PLAY:
		GameInput(gameScene); // ����
		GameUpdate(gameScene);
		GameRender(gameScene);	// �`��	
		break;
	case  GameEnum::Scene::END:
		GameInput(gameScene); // ����
		GameUpdate(gameScene);
		GameRender(gameScene);	// �`��	
		break;
	}
}

//==============================================================================
//!	@fn		GameExit
//!	@brief	�Q�[���I������
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void Game::GameExit()
{

	UninitInput();

	// DirectX11 �O���t�B�b�N�X�I������
	CDirectXGraphics::GetInstance()->Exit();
}

//******************************************************************************
//	End of file.
//******************************************************************************
