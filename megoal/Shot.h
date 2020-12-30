//#pragma once
//#include"quad.h"
//#include"hitbox.h"
//#include"tex.h"
//#include "Object.h"
//
//class Shot :public Object {
//private:
//	enum BLOCKTYPE {
//		NORMALBLOCK = 0,
//		CANONBLOCK,
//	};
//	int Btype;
//
//	int counter = 0;
//	Hitbox *m_box;
//	bool useGravity;
//	Tex m_Texture;
//	ComPtr<ID3D11ShaderResourceView> SrvBlock;
//	ComPtr < ID3D11Resource> ResBlock;
//
//public:
//	Shot();
//	//��P�����F���S���W
//	//��Q�����F�����蔻��̃��[�J�����W
//	bool Init(XMFLOAT2 origin, XMFLOAT2* vertex, const char* texfile, float angle = 0, XMFLOAT2 Speed = XMFLOAT2(0, 0));
//	void Update();
//	void Draw();
//	void Rotate(bool dir);
//	void SetUseGravity(bool g);
//	void SetType(int bt);
//};