#pragma once
#include"quad.h"
#include "Object.h"
#include"hitbox.h"
#include"tex.h"
#include<vector>

#define POTIDEKOSIZE 10

class Hitbox;
class Segment;

//�̂̓����蔻��
static XMFLOAT2 hboxpos[4] = {
	{-2.5f * POTIDEKOSIZE,0 * POTIDEKOSIZE},
	{2.5f * POTIDEKOSIZE,0 * POTIDEKOSIZE},
	{-2.5f * POTIDEKOSIZE,5 * POTIDEKOSIZE},
	{2.5 * POTIDEKOSIZE,5 * POTIDEKOSIZE},
};
//�E���̂�����͂�Ă�
static XMFLOAT2 migipos[4] = {
	{2.5f * POTIDEKOSIZE,-5 * POTIDEKOSIZE},
	{5 * POTIDEKOSIZE,-5 * POTIDEKOSIZE},
	{2.5f * POTIDEKOSIZE,5 * POTIDEKOSIZE},
	{5 * POTIDEKOSIZE,5 * POTIDEKOSIZE},
};
//�����̓����蔻��
static XMFLOAT2 hidaripos[4] = {
	{-5 * POTIDEKOSIZE,-5 * POTIDEKOSIZE},
	{-2.5f * POTIDEKOSIZE,-5 * POTIDEKOSIZE},
	{-5 * POTIDEKOSIZE,5 * POTIDEKOSIZE},
	{-2.5f * POTIDEKOSIZE,5 * POTIDEKOSIZE},
};

class Potideko :public Object {
private:
	//�摜
	Tex m_Ear;
	Tex m_Head;



	Hitbox *m_Centerbox;
	Hitbox *m_Rightbox;
	Hitbox *m_Leftbox;

	ComPtr<ID3D11ShaderResourceView> SrvHead;
	ComPtr < ID3D11Resource> ResHead;
	ComPtr<ID3D11ShaderResourceView> SrvEar;
	ComPtr < ID3D11Resource> ResEar;



public:
	Potideko();
	bool Init(XMFLOAT2 origin, XMFLOAT2* vertex, XMFLOAT2 Speed=XMFLOAT2(0,0));
	void Update();
	void Draw();
	void Rotate(bool dir);
};

