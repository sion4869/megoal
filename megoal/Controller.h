#pragma once
#pragma comment(lib,"XInput.lib")
#include <windows.h>
#include <Xinput.h>
class Controller
{
public:
    enum PAD_STATE{
		// デジタル
        PAD_A,	//	Aボタン
        PAD_B,	//	Bボタン
        PAD_X,	//	Xボタン
        PAD_Y,	//	Yボタン
		PAD_L_SHOULDER,  //	LB
		PAD_R_SHOULDER,	//	RB
		PAD_BACK,	//	Back
		PAD_START,	//	Start
        PAD_UP,		//	十字キーの上
		PAD_DOWN,	//	十字キーの下
		PAD_LEFT,	//	十字キーの左
		PAD_RIGHT,	//	十字キーの右
		PAD_LEFT_THUMB,		//	左スティック
		PAD_RIGHT_THUMB,	//	右スティック
		// アナログ
		PAD_LEFT_U_STICK,	//	左スティックの上
		PAD_LEFT_D_STICK,	//	左スティックの下
		PAD_LEFT_L_STICK,	//	左スティックの左
		PAD_LEFT_R_STICK,	//	左スティックの右
		PAD_RIGHT_U_STICK,	//	右スティックの上
        PAD_RIGHT_D_STICK,	//	右スティックの下
		PAD_RIGHT_L_STICK,	//	右スティックの左
        PAD_RIGHT_R_STICK,	//	右スティックの右
        PAD_L_TRIGGER,	//	左トリガー
		PAD_R_TRIGGER,	//	右トリガー
    };
	enum MODE{
		DIGITAL,
		ANALOG,
	};
public:
    Controller(int playerNum);
    bool IsConnected();
	bool GetPadState(PAD_STATE p_state);

	//	振動
	void EnableVibration(float L_vib, float R_vib);
	void DisableVibration();
private :
	int _controllerNum;
	MODE mode;
	bool GetDigitalState(XINPUT_STATE state, PAD_STATE p_state);
	bool GetAnalogState(XINPUT_STATE state, PAD_STATE p_state);
	void To_XInput(PAD_STATE &p_state);
	void CheckMode(PAD_STATE p_state);
};