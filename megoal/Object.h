#pragma once
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<wrl/client.h>
#include	<string>
#include	"dx11mathutil.h"
#include	"Shader.h"
#include	"shaderhashmap.h"
#include	"DX11Settransform.h"

class Hitbox;

class Object {
protected:
	XMFLOAT2 m_Pos = { 500.0f,500.0f };
	XMFLOAT2 m_PrePos = { 500.0f,500.0f };
	XMFLOAT2 m_Speed = { 0.0f,0.0f };
	float m_Width = 100;
	float m_Height = 100;
	float m_TargetAngle = 0;
	float m_Angle = 0;
	
	std::vector<Hitbox*> m_HitboxList;

public:
	virtual bool Init(XMFLOAT2 origin, XMFLOAT2* vertex,XMFLOAT2 Speed=XMFLOAT2(0,0)) = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	//アクセサ
	XMFLOAT2 GetPos(){return m_Pos;}
	void SetPos(XMFLOAT2 pos){m_Pos=pos;}
	XMFLOAT2 GetPrePos() { return m_PrePos; }
	void SetPrePos(XMFLOAT2 pos) { m_PrePos = pos; }
	XMFLOAT2 GetSpeed() { return m_Speed; }
	void SetSpeed(XMFLOAT2 spd) { m_Speed = spd; }
	XMFLOAT2 GetMove() {
		return XMFLOAT2(m_Pos.x - m_PrePos.x, m_Pos.y - m_PrePos.y);
	}
	XMFLOAT2 GetMove(XMFLOAT2 margin)
	{
		return XMFLOAT2(m_Pos.x - m_PrePos.x - margin.x, m_Pos.y - m_PrePos.y - margin.y);
	}
	void AddHitbox(Hitbox* h) {
		m_HitboxList.push_back(h);
	}

	void AddHitboxList(bool kinetic) {
		if (kinetic)
		{
			extern std::vector<std::vector<Hitbox*>>g_ActiveHitboxList;
			g_ActiveHitboxList.push_back(m_HitboxList);
		}
		else
		{
			extern std::vector<std::vector<Hitbox*>>g_PassiveHitboxList;
			g_PassiveHitboxList.push_back(m_HitboxList);
		}
	}

};