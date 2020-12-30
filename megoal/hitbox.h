#pragma once
#include"quad.h"
#include"Object.h"
#include"potideko.h"
#include<cmath>
#include<vector>

//前方宣言
class Potideko;
class Hitbox;


//ベクトル正規化
XMFLOAT2 Normalize(XMFLOAT2 vec);

//線分ベクトル
class Segment {
public:
	XMFLOAT2 s;//始点
	XMFLOAT2 v;//方向ベクトル
	Segment() {};
	Segment(XMFLOAT2 ts, XMFLOAT2 tv);
	void SetSeg(float ax, float ay, float bx, float by);
	
};
//ベクトル内積
float VecDot(XMFLOAT2 v1, XMFLOAT2 v2);
//ベクトル外積
float VecCross(XMFLOAT2* v1, XMFLOAT2* v2);
float VecCross(XMFLOAT2 v1, XMFLOAT2 v2);
//交差判定
bool ColSegment(
	Segment seg1,          // 線分1
	Segment seg2,          // 線分2
	float* outT1 = 0,       // 線分1の内分比（出力）
	float* outT2 = 0      // 線分2の内分比（出力
);

bool check_Hit(Hitbox a, Hitbox b,bool end);
bool check_Overrap(Hitbox a, Hitbox b);
float Search_path(Hitbox a, Hitbox b);
float RaytoboxCollision(Segment Seg, Hitbox box);//下向きレイとボックスの交差とその距離をだす
void Collision();
void ReCollision();
class Hitbox {
public:
	Object* parent;

	//0:左上 1:右上 2:左下 3:→下
	XMFLOAT2 localpos[4];

	void Init(Object* op,XMFLOAT2* lpos);

	void Move(XMFLOAT2 fix);

	void Rotate90(bool dir);

	Hitbox() {};
};