#pragma once
#include"Object.h"
#include"Block.h"
#include"potideko.h"
//#include"Canon.h"
#include"tex.h"
enum {
	ITA, NANAME, PINO, FLOOR, MINI, LONGITA, GRAV, DEKA, STICK, CANON,
	NUM_OBJ  // �I�u�W�F�N�g�̎��
};

class ObjectManager {
public:

	// ���ǂݍ��ރt�@�C�����̎w���LoadTex�֐����Œ�`����

	static ObjectManager* GetInstance();
	//HRESULT LoadTex(void);  // �e�N�X�`����S���ǂݍ���
	Object* GenObject(XMFLOAT2 Pos, int id,float angle=0, XMFLOAT2 Speed = {0.0f,0.0f});  // ID���w�肵�ă��f���i�̃|�C���^�j���擾����
	~ObjectManager();

private:
	const char* pTexFileNames[1] = {
	"assets/tex1.bmp",
	};

	XMFLOAT2 vertex[NUM_OBJ][10]{ 
	{//���ʂ̃u���b�N
	{-100,-100},
	{100,-100},
	{-100,100},
	{100,100}}, 
	{//naname�u���b�N
	{-200,-100},
	{200,-200},
	{-200,50},
	{200,50}},
	{//�s�m�̃u���b�N
	{-10,-30},
	{10,-30},
	{-30,30},
	{30,30},},
	{//���̃u���b�N
	{-1000,-10},
	{1000,-10},
	{-1000,10},
	{1000,10},},
	{//�������u���b�N
	{-15,-15},
	{15,-15},
	{-15,15},
	{15,15},},
	{//�����u���b�N
	{-300,-30},
	{300,-30},
	{-300,30},
	{300,30},},
	{//�d�̓u���b�N
	{-20,-20},
	{20,-20},
	{-20,20},
	{20,20},},
	{//�f�J�u���b�N
	{-100,-200},
	{100,-200},
	{-100,200},
	{100,200},},
	{//�_�u���b�N
	{-100,-15},
	{100,-15},
	{-100,15},
	{100,15},},
	{//����u���b�N
	{-100,-100},
	{100,-100},
	{-100,100},
	{100,100},},
	};

};