#pragma once
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<wrl/client.h>
#include	<string>
#include	"dx11mathutil.h"
#include	"Shader.h"
#include	"shaderhashmap.h"
#include	"DX11Settransform.h"
#include "input.h"

#define GRAVITY (1.5f);
#define CENTERX 960.0f
#define AREAHALF 480.0f
#define CENTERY 480.0f
#define STAGEBOTTOM 960.0f
#define GRAVBLOCMATH 4
#define POTIDEKOMATH 3
class Hitbox;

class Object {
protected:
	XMFLOAT2 m_Pos = { 500.0f,500.0f };
	XMFLOAT2 m_PrePos = { 500.0f,500.0f };
	XMFLOAT2 m_Speed = { 0.0f,0.0f };
	//float m_Width = 100;
	//float m_Height = 100;
	float m_TargetAngle = 0;
	float m_Angle = 0;
	int m_Math = 0;
	int m_fixMath = 0;

	//ObjectType ObjType;

public:
	std::vector<Hitbox*> m_HitboxList;
	//virtual bool Init(XMFLOAT2 origin, XMFLOAT2* vertex,XMFLOAT2 Speed=XMFLOAT2(0,0)) = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	//アクセサ
	XMFLOAT2 GetPos(){return m_Pos;}
	void SetPos(XMFLOAT2 pos){m_Pos=pos;}
	XMFLOAT2 GetPrePos() { return m_PrePos; }
	void SetPrePos(XMFLOAT2 pos) { m_PrePos = pos; }
	XMFLOAT2 GetSpeed() { return m_Speed; }
	void SetSpeed(XMFLOAT2 spd) { m_Speed = spd; }
	void SetMath(int m) { m_Math = m; }
	int GetMath() { return m_Math; }
	void SetfixMath(int m) { m_fixMath = m; }
	int GetfixMath() { return m_fixMath; }
	void SetTargetAngle(float a) { m_TargetAngle = a; }
	float GetTargetAngle() { return m_TargetAngle; }
	XMFLOAT2 GetMove() {
		return XMFLOAT2(m_Pos.x - m_PrePos.x, m_Pos.y - m_PrePos.y);
	}
	XMFLOAT2 GetMove(XMFLOAT2 margin)
	{
		return XMFLOAT2(m_Pos.x - m_PrePos.x - margin.x, m_Pos.y - m_PrePos.y - margin.y);
	}
	void AddObjectList() {//オブジェクトリストに登録
		extern std::vector<Object*> ObjectList;
		ObjectList.push_back(this);
	}
	void AddHitbox(Hitbox* h) 
	{
		m_HitboxList.push_back(h);
	}

	//void AddHitboxList(bool kinetic) {
	//	if (kinetic)
	//	{
	//		extern std::vector<std::vector<Hitbox*>>g_ActiveHitboxList;
	//		g_ActiveHitboxList.push_back(m_HitboxList);
	//	}
	//	else
	//	{
	//		extern std::vector<std::vector<Hitbox*>>g_PassiveHitboxList;
	//		g_PassiveHitboxList.push_back(m_HitboxList);
	//	}
	//}

};