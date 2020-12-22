#include "Controller.h"

Controller::Controller(int playerNum)
{
	_controllerNum = playerNum -1;
}

bool Controller::IsConnected()
{
	XINPUT_STATE state;	// コントローラ情報取得用
	// 初期化
	ZeroMemory( &state, sizeof(XINPUT_STATE) );

	// 状態取得
	DWORD Result = XInputGetState(_controllerNum, &state);

    if(Result == ERROR_SUCCESS){
        return true;	// 接続されています。
    }else{
        return false;	// 接続されていません。
    }
}


bool Controller::GetPadState(PAD_STATE p_state)
{
	XINPUT_STATE state;	// コントローラ情報取得用
    ZeroMemory( &state, sizeof(XINPUT_STATE) );	// 初期化
    DWORD dwResult;	// 関数結果判定用
	// 情報取得
    dwResult = XInputGetState(_controllerNum,&state);
    if(dwResult == ERROR_SUCCESS){
		CheckMode(p_state);
		switch(mode){
		case ANALOG:
			if(GetAnalogState(state,p_state)){return true;}
			break;
		case DIGITAL:
			if(GetDigitalState(state,p_state)){return true;}
			break;
		}
	}
	return false;
}

bool Controller::GetDigitalState(XINPUT_STATE state, PAD_STATE p_state)
{
	To_XInput(p_state);
	if(state.Gamepad.wButtons & p_state){return true;}
	return false;
}

bool Controller::GetAnalogState(XINPUT_STATE state, PAD_STATE p_state)
{
	// 範囲指定用
	int TRIGGER_DEADZONE = 100;
	int L_STICK_THUMB_DEAD = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	int R_STICK_THUMB_DEAD = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
	switch(p_state){		
	case PAD_L_TRIGGER:
		if(state.Gamepad.bLeftTrigger > TRIGGER_DEADZONE){return true;}
		break;
	case PAD_R_TRIGGER:
		if(state.Gamepad.bRightTrigger > TRIGGER_DEADZONE){return true;}	
		break;
	case PAD_LEFT_U_STICK:
		if(state.Gamepad.sThumbLY >  L_STICK_THUMB_DEAD ){return true;}
		break;
	case PAD_LEFT_D_STICK:
		if(state.Gamepad.sThumbLY < -L_STICK_THUMB_DEAD ){return true;}
		break;
	case PAD_LEFT_R_STICK:
		if(state.Gamepad.sThumbLX >  L_STICK_THUMB_DEAD ){return true;}
		break;
	case PAD_LEFT_L_STICK:
		if(state.Gamepad.sThumbLX < -L_STICK_THUMB_DEAD ){
			return true;
		}
		break;
	case PAD_RIGHT_U_STICK:
		if(state.Gamepad.sThumbRY >  R_STICK_THUMB_DEAD ){return true;}
		break;
	case PAD_RIGHT_D_STICK:
		if(state.Gamepad.sThumbRY < -R_STICK_THUMB_DEAD ){return true;}
		break;
	case PAD_RIGHT_R_STICK:
		if(state.Gamepad.sThumbRX >  R_STICK_THUMB_DEAD ){return true;}
		break;
	case PAD_RIGHT_L_STICK:
		if(state.Gamepad.sThumbRX < -R_STICK_THUMB_DEAD ){return true;}
		break;
	}
	return false;
}

void Controller::To_XInput(PAD_STATE &p_state)
{
	switch(p_state){
	case PAD_A:
		p_state = (PAD_STATE)XINPUT_GAMEPAD_A;
		break;
	case PAD_B:
		p_state = (PAD_STATE)XINPUT_GAMEPAD_B;
		break;
	case PAD_X:
		p_state = (PAD_STATE)XINPUT_GAMEPAD_X;
		break;
	case PAD_Y:
		p_state = (PAD_STATE)XINPUT_GAMEPAD_Y;
		break;
	case PAD_L_SHOULDER:
		p_state = (PAD_STATE)XINPUT_GAMEPAD_LEFT_SHOULDER;
		break;
	case PAD_R_SHOULDER:
		p_state = (PAD_STATE)XINPUT_GAMEPAD_RIGHT_SHOULDER;
		break;
	case PAD_BACK:
		p_state = (PAD_STATE)XINPUT_GAMEPAD_BACK;
		break;
	case PAD_START:
		p_state = (PAD_STATE)XINPUT_GAMEPAD_START;
		break;
	case PAD_UP:
		p_state = (PAD_STATE)XINPUT_GAMEPAD_DPAD_UP;
		break;
	case PAD_DOWN:
		p_state = (PAD_STATE)XINPUT_GAMEPAD_DPAD_DOWN;
		break;
	case PAD_LEFT:
		p_state = (PAD_STATE)XINPUT_GAMEPAD_DPAD_LEFT;
		break;
	case PAD_RIGHT:
		p_state = (PAD_STATE)XINPUT_GAMEPAD_DPAD_RIGHT;
		break;
	case PAD_LEFT_THUMB:
		p_state = (PAD_STATE)XINPUT_GAMEPAD_LEFT_THUMB;
		break;
	case PAD_RIGHT_THUMB:
		p_state = (PAD_STATE)XINPUT_GAMEPAD_RIGHT_THUMB;
		break;
	}
}

void Controller::CheckMode(PAD_STATE p_state){
	switch(p_state){
	case PAD_LEFT_U_STICK:
	case PAD_LEFT_D_STICK:
	case PAD_LEFT_L_STICK:
	case PAD_LEFT_R_STICK:
	case PAD_RIGHT_U_STICK:
	case PAD_RIGHT_D_STICK:
	case PAD_RIGHT_L_STICK:
	case PAD_RIGHT_R_STICK:
	case PAD_L_TRIGGER:
	case PAD_R_TRIGGER:
		mode = ANALOG;	//	アナログモード
		break;
	default:
		mode = DIGITAL;	//	デジタルモード
		break;
	}
}

//--------------------------------------------------------------
//	振動開始
//	@param [in] L_vib	左振動値(0.0f ~ 1.0f)
//	@param [in] R_vib	右振動値(0.0f ~ 1.0f)
//--------------------------------------------------------------
void Controller::EnableVibration(float L_vib, float R_vib)
{
	//	振動値を0.0~1.0fから 0 ~ 65535に変換
	L_vib = 65535 * L_vib;
	R_vib = 65535 * R_vib;
	//	振動用の構造体
	XINPUT_VIBRATION	vibration;
	//	初期化
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	//	振動値代入
	vibration.wLeftMotorSpeed = L_vib;
	vibration.wRightMotorSpeed = R_vib;
	//	結果転送
	XInputSetState(_controllerNum, &vibration);
}
//--------------------------------------------------------------
//!	振動停止
//--------------------------------------------------------------
void Controller::DisableVibration()
{
	//	振動用の構造体
	XINPUT_VIBRATION	vibration;
	//	初期化
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	//	振動値代入
	vibration.wLeftMotorSpeed = 0;
	vibration.wRightMotorSpeed = 0;
	//	結果転送
	XInputSetState(_controllerNum, &vibration);
}