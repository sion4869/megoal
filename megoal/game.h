//*****************************************************************************
//!	@file	game.h
//!	@brief	ゲーム処理
//!	@note	
//!	@author
//*****************************************************************************
#pragma once


#define		SCREEN_X		1920
#define		SCREEN_Y		1080
#define		FULLSCREEN      0

namespace GameEnum {
	enum Image {
		TITLE,
		STAG2,
		RESULT,
		POTIDEKO,
		IMAGE_MAX
	};

	enum class Scene {
		TITLE,
		PLAY,
		PLAY2,
		END
	};
}

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------

void DrawFPS(HWND hwnd, int dwFrameCount);


class Game {
private:
	GameEnum::Scene gameScene = GameEnum::Scene::TITLE;

public:
	Game() {};
	bool GameInit(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen);
	void GameMain();
	void GameExit();
	void GameInput(GameEnum::Scene scene);
	void GameUpdate(GameEnum::Scene scene);
	void GameRender(GameEnum::Scene scene);

};

//******************************************************************************
//	End of file.
//******************************************************************************
