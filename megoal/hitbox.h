#pragma once
#include"quad.h"
#include"Object.h"
#include"potideko.h"
#include<cmath>
#include<vector>

//�O���錾
class Potideko;
class Hitbox;


//�x�N�g�����K��
XMFLOAT2 Normalize(XMFLOAT2 vec);

//�����x�N�g��
class Segment {
public:
	XMFLOAT2 s;//�n�_
	XMFLOAT2 v;//�����x�N�g��
	Segment() {};
	Segment(XMFLOAT2 ts, XMFLOAT2 tv);
	void SetSeg(float ax, float ay, float bx, float by);
	
};
//�x�N�g������
float VecDot(XMFLOAT2 v1, XMFLOAT2 v2);
//�x�N�g���O��
float VecCross(XMFLOAT2* v1, XMFLOAT2* v2);
float VecCross(XMFLOAT2 v1, XMFLOAT2 v2);
//��������
bool ColSegment(
	Segment seg1,          // ����1
	Segment seg2,          // ����2
	float* outT1 = 0,       // ����1�̓�����i�o�́j
	float* outT2 = 0      // ����2�̓�����i�o��
);

bool check_Hit(Hitbox a, Hitbox b,bool end);
bool check_Overrap(Hitbox a, Hitbox b);
float Search_path(Hitbox a, Hitbox b);
float RaytoboxCollision(Segment Seg, Hitbox box);//���������C�ƃ{�b�N�X�̌����Ƃ��̋���������
void Collision();
void ReCollision();
class Hitbox {
public:
	Object* parent;

	//0:���� 1:�E�� 2:���� 3:����
	XMFLOAT2 localpos[4];

	void Init(Object* op,XMFLOAT2* lpos);

	void Move(XMFLOAT2 fix);

	void Rotate90(bool dir);

	Hitbox() {};
};