#pragma once
#include"quad.h"
#include "Object.h"
#include"hitbox.h"
#include"tex.h"
#include<typeinfo>
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

	enum STATE {
		NEUTRAL,
		SAKASA,
		TURN,
		JUMP,
		FALL,
		STOP,
	} m_state;
	//�摜
	Tex m_Face;
	Tex m_Head;
	Tex m_Foot;

	//�����蔻��
	Hitbox *m_Centerbox;
	Hitbox *m_Rightbox;
	Hitbox *m_Leftbox;

	float m_footPos;//���̌����ڂ̒����Ǘ�
	Segment *FootRangeL[2];
	Segment *FootRangeR[2];
	Segment *SwitchRange;

	ComPtr<ID3D11ShaderResourceView> SrvHead;
	ComPtr < ID3D11Resource> ResHead;
	ComPtr<ID3D11ShaderResourceView> SrvFace;
	ComPtr < ID3D11Resource> ResFace;
	ComPtr<ID3D11ShaderResourceView> SrvFoot;
	ComPtr < ID3D11Resource> ResFoot;

public:
	Potideko();
	~Potideko();
	bool Init(XMFLOAT2 origin);
	void Update();
	bool isGrounded(float* out);
	void Draw();
	void Rotate(bool dir);
	bool FootSpring();
	bool EarSpring();
	bool CheckSwitch();
};

