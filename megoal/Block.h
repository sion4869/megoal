#pragma once
#include"quad.h"
#include"hitbox.h"
#include"tex.h"
#include "Object.h"

class Block :public Object {
private:
	Hitbox *m_box;

	Tex m_End;

	Tex m_Texture;
	ComPtr<ID3D11ShaderResourceView> SrvBlock;
	ComPtr < ID3D11Resource> ResBlock;

	ComPtr<ID3D11ShaderResourceView> SrvEnd;
	ComPtr < ID3D11Resource> ResEnd;

public:
	Block();
	//��P�����F���S���W
	//��Q�����F�����蔻��̃��[�J�����W
	bool Init(XMFLOAT2 origin,XMFLOAT2* vertex,XMFLOAT2 Speed=XMFLOAT2(0,0));
	void Update();
	void Draw();
};