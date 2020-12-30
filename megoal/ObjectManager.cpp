#include "ObjectManager.h"

ObjectManager * ObjectManager::GetInstance()
{
	static ObjectManager instance;
	return &instance;
}

Object * ObjectManager::GenObject(XMFLOAT2 Pos,int id,float angle,XMFLOAT2 Speed)
{

	Object* pObj=nullptr;
	switch (id)
	{
	case ITA: 
	{
		pObj = new Block;
		Block* pb = (Block*)pObj;
		pb->Init(Pos, vertex[id], pTexFileNames[0], angle,Speed);
		break; 
	}
	case NANAME:
	{
		pObj = new Block;
		Block* pb = (Block*)pObj;
		pb->Init(Pos, vertex[id], pTexFileNames[0], angle, Speed);
		break;
	}
	case PINO:
	{
		pObj = new Block;
		Block* pb = (Block*)pObj;
		pb->Init(Pos, vertex[id], pTexFileNames[0], angle, Speed);
		break;
	}
	case FLOOR:
	{
		pObj = new Block;
		Block* pb = (Block*)pObj;
		pb->Init(Pos, vertex[id], pTexFileNames[0], angle, Speed);
		break;
	}
	case MINI:
	{
		pObj = new Block;
		Block* pb = (Block*)pObj;
		pb->Init(Pos, vertex[id], pTexFileNames[0], angle, Speed);
		break;
	}
	case LONGITA:
	{
		pObj = new Block;
		Block* pb = (Block*)pObj;
		pb->Init(Pos, vertex[id], pTexFileNames[0], angle, Speed);
		break;
	}
	case GRAV:
	{
		pObj = new Block;
		Block* pb = (Block*)pObj;
		pb->Init(Pos, vertex[id], pTexFileNames[0], angle, Speed);
		pb->SetUseGravity(1);
		pb->SetMath(GRAVBLOCMATH);
		break;
	}
	case DEKA:
	{
		pObj = new Block;
		Block* pb = (Block*)pObj;
		pb->Init(Pos, vertex[id], pTexFileNames[0], angle, Speed);
		break;
	}
	case STICK:
	{
		pObj = new Block;
		Block* pb = (Block*)pObj;
		pb->Init(Pos, vertex[id], pTexFileNames[0], angle, Speed);
		break;
	}
	case CANON:
	{
		pObj = new Block;
		Block* pb = (Block*)pObj;
		pb->Init(Pos, vertex[id], pTexFileNames[0], angle, Speed);
		pb->SetType(1);

		break;
	}
	}
	//ここでオブジェクトを作って渡すよ
	return pObj;
}


ObjectManager::~ObjectManager()
{
	for (int i = 0; i < NUM_OBJ; i++)
	{
		//delete m_texs[i];
	}
}
