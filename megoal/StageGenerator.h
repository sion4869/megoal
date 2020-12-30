#pragma once
#include"Object.h"
#include"potideko.h"
#include"Block.h"
#include"ObjectManager.h"

typedef struct {
	//いるもん　物のタイプ　位置　角度
	int objtype;
	XMFLOAT2 Pos;
	float Angle;
}ObjectInfo;

typedef struct
{
	//ステージのブロックの数
	ObjectInfo ObjInfo[50];
	int num;
}StageInfo;

//StageInfoを受け取ってステージを生成するクラス() 
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
			//オブジェクトにオブジェクト番号と位置を入れたらいい感じになる魔法をかける
			ObjectList.push_back(
				ObjectManager::GetInstance()->GenObject(Sinfo.ObjInfo[i].Pos, Sinfo.ObjInfo[i].objtype, Sinfo.ObjInfo[i].Angle)
			);
		}
	}
};