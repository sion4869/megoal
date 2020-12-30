#pragma once
#include"Object.h"
#include"potideko.h"
#include"Block.h"
#include"ObjectManager.h"

typedef struct {
	//�������@���̃^�C�v�@�ʒu�@�p�x
	int objtype;
	XMFLOAT2 Pos;
	float Angle;
}ObjectInfo;

typedef struct
{
	//�X�e�[�W�̃u���b�N�̐�
	ObjectInfo ObjInfo[50];
	int num;
}StageInfo;

//StageInfo���󂯎���ăX�e�[�W�𐶐�����N���X() 
class StageGenerator {
public:
	static StageGenerator * GetInstance()
	{
		static StageGenerator instance;
		return &instance;
	}
	void GenStage(StageInfo Sinfo){
		extern std::vector<Object*>ObjectList;
		Object* poti = new Potideko;
		Potideko* pp = (Potideko*)poti;
		pp->Init(XMFLOAT2(200, 200));
		ObjectList.push_back(poti);
		
		ObjectList.push_back(
			ObjectManager::GetInstance()->GenObject(XMFLOAT2(960.0f,STAGEBOTTOM+10.0f), 3)
		);
		for (int i = 0; i < Sinfo.num; i++)
		{
			//�I�u�W�F�N�g�ɃI�u�W�F�N�g�ԍ��ƈʒu����ꂽ�炢�������ɂȂ閂�@��������
			ObjectList.push_back(
				ObjectManager::GetInstance()->GenObject(Sinfo.ObjInfo[i].Pos, Sinfo.ObjInfo[i].objtype, Sinfo.ObjInfo[i].Angle)
			);
		}
	}
};