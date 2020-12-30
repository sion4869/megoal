#pragma once
#include"Object.h"
#include"Block.h"
#include"potideko.h"
//#include"Canon.h"
#include"tex.h"
enum {
	ITA, NANAME, PINO, FLOOR, MINI, LONGITA, GRAV, DEKA, STICK, CANON,
	NUM_OBJ  // オブジェクトの種類
};

class ObjectManager {
public:

	// ※読み込むファイル名の指定はLoadTex関数内で定義する

	static ObjectManager* GetInstance();
	//HRESULT LoadTex(void);  // テクスチャを全部読み込む
	Object* GenObject(XMFLOAT2 Pos, int id,float angle=0, XMFLOAT2 Speed = {0.0f,0.0f});  // IDを指定してモデル（のポインタ）を取得する
	~ObjectManager();

private:
	const char* pTexFileNames[1] = {
	"assets/tex1.bmp",
	};

	XMFLOAT2 vertex[NUM_OBJ][10]{ 
	{//普通のブロック
	{-100,-100},
	{100,-100},
	{-100,100},
	{100,100}}, 
	{//nanameブロック
	{-200,-100},
	{200,-200},
	{-200,50},
	{200,50}},
	{//ピノのブロック
	{-10,-30},
	{10,-30},
	{-30,30},
	{30,30},},
	{//床のブロック
	{-1000,-10},
	{1000,-10},
	{-1000,10},
	{1000,10},},
	{//小さいブロック
	{-15,-15},
	{15,-15},
	{-15,15},
	{15,15},},
	{//長い板ブロック
	{-300,-30},
	{300,-30},
	{-300,30},
	{300,30},},
	{//重力ブロック
	{-20,-20},
	{20,-20},
	{-20,20},
	{20,20},},
	{//デカブロック
	{-100,-200},
	{100,-200},
	{-100,200},
	{100,200},},
	{//棒ブロック
	{-100,-15},
	{100,-15},
	{-100,15},
	{100,15},},
	{//放題ブロック
	{-100,-100},
	{100,-100},
	{-100,100},
	{100,100},},
	};

};