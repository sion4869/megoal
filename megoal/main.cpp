//*****************************************************************************
//!	@file	main.cpp
//!	@brief	
//!	@note	DX11 �ЂȌ`
//!	@author	
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include <windows.h>
#include <crtdbg.h>
#include "game.h"

//-----------------------------------------------------------------------------
// �}�N���̒�`
//-----------------------------------------------------------------------------
#define		NAME			"DX11"
#define		TITLE			"DX21 �A�j���[�V����"

//-----------------------------------------------------------------------------
// �����N���C�u�����̐ݒ�
//-----------------------------------------------------------------------------
#pragma comment(lib, "winmm.lib")			// �����N�Ώۃ��C�u������winmm.lib��ǉ�
#pragma comment(lib, "d3d11.lib")			// �����N�Ώۃ��C�u������d3d11.lib��ǉ�
#pragma comment(lib, "dxgi.lib")			// �����N�Ώۃ��C�u������dxgi.lib��ǉ�
#pragma comment(lib, "D3DCompiler.lib")		// �����N�Ώۃ��C�u������d3dcompiler.lib��ǉ�
#pragma comment(lib, "DirectXTex.lib")		// �����N�Ώۃ��C�u������DirectXTex.lib��ǉ�
//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);			// �E�C���h�E�v���V�[�W��

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
int g_nCountFPS = 0;				// �e�o�r�J�E���^

//==============================================================================
//!	@fn		WinMain
//!	@brief	�G���g���|�C���g
//!	@param	�C���X�^���X�n���h��
//!	@param	�Ӗ��Ȃ�
//!	@param	�N�����̈���������
//!	@param	�E�C���h�E�\�����[�h
//!	@retval	TRUE�@�����I��/FALSE�@���s�I��
//!	@note	
//==============================================================================
int APIENTRY WinMain(HINSTANCE 	hInstance, 		// �A�v���P�[�V�����̃n���h��
					 HINSTANCE 	hPrevInstance,	// ����Windows�o�[�W�����ł͎g���Ȃ�
					 LPSTR 		lpszArgs, 		// �N�����̈����i������j
					 int 		nWinMode)		// �E�C���h�E�\�����[�h
{
	HWND			hwnd;						// �E�C���h�E�n���h��
	MSG				msg;						// ���b�Z�[�W�\����
	WNDCLASSEX		wcex;						// �E�C���h�E�N���X�\����
	int				width = SCREEN_X;			// �E�C���h�E�̕� �v�Z�p���[�N
	int				height = SCREEN_Y;			// �E�C���h�E�̍��� �v�Z�p���[�N

	DWORD dwExecLastTime;		// �Ō�Ɏ��s��������		
	DWORD dwFPSLastTime;		// �Ō�Ɍv����������
	DWORD dwCurrentTime;		// ���ݎ���
	DWORD dwFrameCount;			// �t���[����

	// ���������[�N�����m
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// �E�C���h�E�N���X���̃Z�b�g
	wcex.hInstance		= hInstance;			// �C���X�^���X�l�̃Z�b�g
	wcex.lpszClassName	= NAME;					// �N���X��
	wcex.lpfnWndProc	= (WNDPROC)WndProc;		// �E�C���h�E���b�Z�[�W�֐�
	wcex.style			= 0;					// �E�C���h�E�X�^�C��
	wcex.cbSize 		= sizeof(WNDCLASSEX);	// �\���̂̃T�C�Y
	wcex.hIcon			= LoadIcon((HINSTANCE)NULL, IDI_APPLICATION);	// ���[�W�A�C�R��
	wcex.hIconSm		= LoadIcon((HINSTANCE)NULL, IDI_WINLOGO);		// �X���[���A�C�R��
	wcex.hCursor		= LoadCursor((HINSTANCE)NULL, IDC_ARROW);		// �J�[�\���X�^�C��
	wcex.lpszMenuName	= 0; 					// ���j���[�Ȃ�
	wcex.cbClsExtra		= 0;					// �G�L�X�g���Ȃ�
	wcex.cbWndExtra		= 0;					
	wcex.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);			// �w�i�F��

	if (!RegisterClassEx(&wcex)) return FALSE;	// �E�C���h�E�N���X�̓o�^

	RECT	rWindow, rClient;

	hwnd = CreateWindow(
			NAME,							// �E�B���h�E�N���X�̖��O
			TITLE,							// �^�C�g��
			WS_CAPTION | WS_SYSMENU,		// �E�B���h�E�X�^�C��
			0, 0,							// �E�B���h�E�ʒu �c, ��(���ƂŒ����Ɉړ������܂�)
			SCREEN_X, SCREEN_Y,				// �E�B���h�E�T�C�Y
			HWND_DESKTOP,					// �e�E�B���h�E�Ȃ�
			(HMENU)NULL,					// ���j���[�Ȃ�
			hInstance,						// �C���X�^���X�n���h��
			(LPVOID)NULL);					// �ǉ������Ȃ�
		
	// �E�C���h�E�T�C�Y���Čv�Z�iMetrics�����ł́A�t���[���f�U�C���ŃN���C�A���g�̈�T�C�Y���ς���Ă��܂��̂Łj
	GetWindowRect(hwnd, &rWindow);
	GetClientRect(hwnd, &rClient);
	width  = (rWindow.right - rWindow.left) - (rClient.right - rClient.left) + SCREEN_X;
	height = (rWindow.bottom - rWindow.top) - (rClient.bottom - rClient.top) + SCREEN_Y;

	// �E�C���h�E�̕\���ʒu���Z�b�g
	SetWindowPos(
		hwnd,
		NULL,
		GetSystemMetrics(SM_CXSCREEN)/2-width/2,
		GetSystemMetrics(SM_CYSCREEN)/2-height/2,
		width-1,
		height-1,
		SWP_NOZORDER);

	if (!hwnd) return false;

	timeBeginPeriod(1);							// �^�C�}�̕���\�͂��Pms�ɂ���
	dwExecLastTime = dwFPSLastTime = timeGetTime();
	dwCurrentTime = dwFrameCount = 0;
	// �E�C���h�E��\������
	ShowWindow(hwnd, nWinMode);
	UpdateWindow(hwnd);

	Game game;

	// �Q�[���̏�������
	if(!game.GameInit(hInstance,hwnd,SCREEN_X,SCREEN_Y,FULLSCREEN)){
		game.GameExit();
		MessageBox(hwnd,"ERROR!","GameInit Error",MB_OK);
		return false;
	}

	// ���b�Z�[�W���[�v
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)	// ���b�Z�[�W���擾���Ȃ������ꍇ"0"��Ԃ�
		{// Windows�̏���
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂�āAWM_QUIT���b�Z�[�W�������烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{// DirectX�̏���
		 // �e�o�r��艻����

			// ���ݎ��Ԃ��擾
			dwCurrentTime = timeGetTime();

			// 0.5�b���ƂɎ��s
			if ((dwCurrentTime - dwFPSLastTime) > 500) {

				// FPS���v�Z
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				// �e�o�r�v�����ԂɌ������Ԃ��Z�b�g
				dwFPSLastTime = dwCurrentTime;

				// �t���[�������[���N���A
				dwFrameCount = 0;
			}

			// �P�U�~���b�o�߂�����
			if ((dwCurrentTime - dwExecLastTime) >= (1000.0f / 60.0f)) {

				// �ŏI���s���ԂɌ��ݎ��Ԃ��Z�b�g
				dwExecLastTime = dwCurrentTime;

				// �Q�[�����C������
				game.GameMain();

				dwFrameCount++;					// �t���[�����J�E���g�A�b�v
			}
		}
	}

	timeEndPeriod(1);							// �^�C�}�̕���\�͂��Ƃɖ߂�

	game.GameExit();									// �Q�[���̏I������

	return (int)msg.wParam;
}

//==============================================================================
//!	@fn		WindowProc
//!	@brief	�E�C���h�E�v���V�[�W��
//!	@param	�E�C���h�E�n���h��
//!	@param	���b�Z�[�W
//!	@param	W�p�����[�^
//!	@param	L�p�����[�^
//!	@retval	�I����
//==============================================================================
LRESULT WINAPI WndProc(	HWND hwnd, 		// �E�B���h�E�n���h��
						UINT message,	// ���b�Z�[�W���ʎq
						WPARAM wParam,	// �t�я��P
						LPARAM lParam)	// �t�я��Q
{

	switch( message ){

		case WM_KEYDOWN:						// �L�[�{�[�h����	
			switch(wParam){
				case VK_ESCAPE:
					// �E�C���h�E���I��������
					DestroyWindow(hwnd);
					break;
			}
			break;
		case WM_DESTROY:						// �E�C���h�E�I����
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}

//******************************************************************************
//	End of file.
//******************************************************************************
