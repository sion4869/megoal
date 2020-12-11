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
#include "Sprite2D.h"


#define rep(n) for(int i =0;i<n;i++)
//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
Potideko g_Potideko;
Block g_block1;
Block g_block2;
Block g_block3;

Sprite2D* g_title;

std::vector<Object*> ObjectList;
std::vector<std::vector<Hitbox*>> g_ActiveHitboxList;
std::vector<std::vector<Hitbox*>> g_PassiveHitboxList;

using Microsoft::WRL::ComPtr;
ComPtr<ID3D11ShaderResourceView> g_srv[GameEnum::Image::IMAGE_MAX]; //�e�N�X�`�����V�F�[�_�[�ŗ��p���邽�߂ɕK�v
ComPtr<ID3D11Resource> g_resource[GameEnum::Image::IMAGE_MAX];      //�e�N�X�`���I�u�W�F�N�g


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

	//�����蔻��
	XMFLOAT2 ppos[4] = {
		{-5 * POTIDEKOSIZE,-5 * POTIDEKOSIZE},
		{5 * POTIDEKOSIZE,-5 * POTIDEKOSIZE},
		{-5 * POTIDEKOSIZE,5 * POTIDEKOSIZE},
		{5 * POTIDEKOSIZE,5 * POTIDEKOSIZE},
	};
	//�����蔻��
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

	//�����蔻��
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

	g_title = new Sprite2D();
	HRESULT hr2 = g_title->Initialize();
	if (FAILED(hr2)) return false;

	g_title->LoadTexture("assets/TitleBack.png");
	if (FAILED(hr2)) return false;

	Sprite2D::CreateShader();
	if (FAILED(hr2)) return false;

	return	true;
}

//==============================================================================
//!	@fn		GameInput
//!	@brief	�Q�[�����͌��m����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void Game::GameInput(GameEnum::Scene scene) {
	/*UpdateInput();*/

	switch (scene) {
	case GameEnum::Scene::TITLE: {
		//�}�E�X�����N���b�N�����Ƃ�
		if (GetKeyboardPress(DIK_T)) {
			gameScene = GameEnum::Scene::PLAY;
			//PlaySound(SOUND_LABEL_BGM000);
		}
		break; }


	case GameEnum::Scene::PLAY: {  //firststage

		break; }
	}
}

//==============================================================================
//!	@fn		GameUpdate
//!	@brief	�Q�[���X�V����
//!	@param�@	
//!	@retval	�Ȃ�
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

		////�����蔻�菈���@�Ȃ񂾂���
		//for (auto a : g_ActiveHitboxList)
		//	for (auto active : a)
		//		for (auto p : g_PassiveHitboxList)
		//			for (auto passive : p)
		//				check_Hit(*active, *passive,1);
		Collition();
		break; }

	}
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

	

	switch (gameScene)
	{
	case GameEnum::Scene::TITLE: {
		g_title->Draw();
		break; }


	case GameEnum::Scene::PLAY: {
		g_Potideko.Draw();
		g_block1.Draw();
		g_block2.Draw();
		g_block3.Draw();
		break; }
	
	}

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
	Sprite2D::ReleaseShader();
	if (g_title != nullptr) {
		delete g_title;
		g_title = nullptr;
	}

	UninitInput();

	// DirectX11 �O���t�B�b�N�X�I������
	CDirectXGraphics::GetInstance()->Exit();
}

//******************************************************************************
//	End of file.
//******************************************************************************
