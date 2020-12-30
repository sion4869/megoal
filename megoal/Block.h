#pragma once
#include"quad.h"
#include"hitbox.h"
#include"tex.h"
#include "Object.h"

class Block :public Object {
private:
	enum STATE {
		NEUTRAL,
		TURN,
	} m_state;

	enum BLOCKTYPE {
		NORMALBLOCK = 0,
		CANONBLOCK,
		MOVEBLOCK,
	};
	int Btype;

	int counter = 0;
	Hitbox *m_box;
	bool useGravity;
	Tex m_Texture;
	ComPtr<ID3D11ShaderResourceView> SrvBlock;
	ComPtr < ID3D11Resource> ResBlock;

public:
	Block();
	//��P�����F���S���W
	//��Q�����F�����蔻��̃��[�J�����W
	bool Init(XMFLOAT2 origin,XMFLOAT2* vertex, const char* texfile,float angle=0,XMFLOAT2 Speed=XMFLOAT2(0,0));
	void Update();
	void Draw();
	void Rotate(bool dir);
	void SetUseGravity(bool g);
	bool GetUseGravity();
	void FixTexVertex(XMFLOAT2 fix);

	void SetType(int bt);
};