#include"hitbox.h"
#include"Object.h"
#include"potideko.h"
#include<cmath>
#include<vector>

float debug = 0;
XMFLOAT2 Normalize(XMFLOAT2 vec) {
	float mag = 1 / sqrt((vec.x*vec.x + vec.y*vec.y));
	vec.x *= mag;
	vec.y *= mag;

	return vec;
}

Segment::Segment(XMFLOAT2 ts, XMFLOAT2 tv) {
	s = ts;
	v = tv;
}

void Segment::SetSeg(float ax, float ay, float bx, float by) {
	s.x = ax;
	s.y = ay;
	v.x = bx - ax;
	v.y = by - ay;
}

void Hitbox::Init(bool kinetic, Object* op, XMFLOAT2* lpos) {
	isKinetic = kinetic;

	parent = op;

	for (int i = 0; i < 4; i++)
		localpos[i] = lpos[i];
	parent->AddHitbox(this);
}

//dir 0:右　1:左
void Hitbox::Rotate90(bool dir)
{
	XMFLOAT2 tmp;
	float tmp2;
	if (dir == 0)
	{
		for (int i = 0; i < 4; i++)
		{
			tmp2 = localpos[i].x;
			localpos[i].x = -localpos[i].y;
			localpos[i].y = tmp2;
		}
		tmp = localpos[0];
		localpos[0] = localpos[2];
		localpos[2] = localpos[3];
		localpos[3] = localpos[1];
		localpos[1] = tmp;
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			tmp2 = localpos[i].x;
			localpos[i].x = localpos[i].y;
			localpos[i].y = -tmp2;
		}

		tmp = localpos[0];
		localpos[0] = localpos[1];
		localpos[1] = localpos[3];
		localpos[3] = localpos[2];
		localpos[2] = tmp;
	}


}
//内積
float VecDot(XMFLOAT2 v1, XMFLOAT2 v2) {
	return v1.x * v2.x + v1.y *  v2.y;
}
//外積
float VecCross(XMFLOAT2* v1, XMFLOAT2* v2) {
	return v1->x * v2->y - v1->y * v2->x;
}
float VecCross(XMFLOAT2 v1, XMFLOAT2 v2) {
	return v1.x * v2.y - v1.y * v2.x;
}

//交差判定
bool ColSegment(
	Segment seg1,          // 線分1
	Segment seg2,          // 線分2
	float* outT1,       // 線分1の内分比（出力）
	float* outT2        // 線分2の内分比（出力)
) {
	XMFLOAT2 v;
	v.x = seg2.s.x - seg1.s.x;
	v.y = seg2.s.y - seg1.s.y;
	float Crs_v1_v2 = VecCross(&seg1.v, &seg2.v);
	if (Crs_v1_v2 == 0.0f)
	{
		//平行状態
		return false;
	}

	float Crs_v_v1 = VecCross(&v, &seg1.v);
	float Crs_v_v2 = VecCross(&v, &seg2.v);

	float t1 = Crs_v_v2 / Crs_v1_v2;
	float t2 = Crs_v_v1 / Crs_v1_v2;

	if (outT1)
		*outT1 = Crs_v_v2 / Crs_v1_v2;
	if (outT2)
		*outT2 = Crs_v_v1 / Crs_v1_v2;

	const float eps = 0.00001f;
	if (t1 + eps < 0 || t1 - eps>1 || t2 + eps < 0 || t2 - eps>1)
	{
		//交差していない
		return false;
	}
	return true;
}

bool check_Hit(Hitbox a, Hitbox b, bool end) {
	//相対移動ベクトルを求める
	XMFLOAT2 aVec = a.parent->GetMove(b.parent->GetMove());

	//aの位置　ｂの位置//ここら辺クラスで保持してたほうがええかも
	XMFLOAT2 apos = a.parent->GetPos();
	XMFLOAT2 bpos = b.parent->GetPos();

	//aの相対移動ベクトルと始点のセグメントを作る
	Segment aSeg;
	aSeg.SetSeg(apos.x - aVec.x, apos.y - aVec.y, apos.x, apos.y);
	//bのミンコフスキー差のセグメントを作る
	Segment bSeg[16];

	//左上から時計回りに
	XMFLOAT2 mink[12];

	mink[0] = XMFLOAT2(bpos.x + b.localpos[0].x - a.localpos[1].x, bpos.y + b.localpos[0].y - a.localpos[1].y);
	mink[1] = XMFLOAT2(bpos.x + b.localpos[0].x - a.localpos[3].x, bpos.y + b.localpos[0].y - a.localpos[3].y);
	mink[2] = XMFLOAT2(bpos.x + b.localpos[0].x - a.localpos[2].x, bpos.y + b.localpos[0].y - a.localpos[2].y);
	mink[3] = XMFLOAT2(bpos.x + b.localpos[1].x - a.localpos[3].x, bpos.y + b.localpos[1].y - a.localpos[3].y);
	mink[4] = XMFLOAT2(bpos.x + b.localpos[1].x - a.localpos[2].x, bpos.y + b.localpos[1].y - a.localpos[2].y);
	mink[5] = XMFLOAT2(bpos.x + b.localpos[1].x - a.localpos[0].x, bpos.y + b.localpos[1].y - a.localpos[0].y);
	mink[6] = XMFLOAT2(bpos.x + b.localpos[3].x - a.localpos[2].x, bpos.y + b.localpos[3].y - a.localpos[2].y);
	mink[7] = XMFLOAT2(bpos.x + b.localpos[3].x - a.localpos[0].x, bpos.y + b.localpos[3].y - a.localpos[0].y);
	mink[8] = XMFLOAT2(bpos.x + b.localpos[3].x - a.localpos[1].x, bpos.y + b.localpos[3].y - a.localpos[1].y);
	mink[9] = XMFLOAT2(bpos.x + b.localpos[2].x - a.localpos[0].x, bpos.y + b.localpos[2].y - a.localpos[0].y);
	mink[10] = XMFLOAT2(bpos.x + b.localpos[2].x - a.localpos[1].x, bpos.y + b.localpos[2].y - a.localpos[1].y);
	mink[11] = XMFLOAT2(bpos.x + b.localpos[2].x - a.localpos[3].x, bpos.y + b.localpos[2].y - a.localpos[3].y);


	//交差してたら0~1になるはず
	float T1 = 2;
	float Ttmp = 2;

	//
	int counter = 20;
	XMFLOAT2 aVec_2;

	//法線ベクトル
	XMFLOAT2 normal;

	//正規化したbSeg.v
	XMFLOAT2 normalizedB;

	//衝突後に修正するベクトル
	XMFLOAT2 fixVec;

	//aVec_2とbSegの内積
	float Dot;
	bSeg[0].SetSeg(mink[0].x, mink[0].y, mink[1].x, mink[1].y);
	bSeg[1].SetSeg(mink[1].x, mink[1].y, mink[2].x, mink[2].y);
	bSeg[2].SetSeg(mink[1].x, mink[1].y, mink[3].x, mink[3].y);
	bSeg[3].SetSeg(mink[2].x, mink[2].y, mink[4].x, mink[4].y);
	bSeg[4].SetSeg(mink[3].x, mink[3].y, mink[4].x, mink[4].y);
	bSeg[5].SetSeg(mink[4].x, mink[4].y, mink[5].x, mink[5].y);
	bSeg[6].SetSeg(mink[4].x, mink[4].y, mink[6].x, mink[6].y);
	bSeg[7].SetSeg(mink[5].x, mink[5].y, mink[7].x, mink[7].y);
	bSeg[8].SetSeg(mink[6].x, mink[6].y, mink[7].x, mink[7].y);
	bSeg[9].SetSeg(mink[7].x, mink[7].y, mink[8].x, mink[8].y);
	bSeg[10].SetSeg(mink[7].x, mink[7].y, mink[9].x, mink[9].y);
	bSeg[11].SetSeg(mink[8].x, mink[8].y, mink[10].x, mink[10].y);
	bSeg[12].SetSeg(mink[9].x, mink[9].y, mink[10].x, mink[10].y);
	bSeg[13].SetSeg(mink[10].x, mink[10].y, mink[11].x, mink[11].y);
	bSeg[14].SetSeg(mink[10].x, mink[10].y, mink[0].x, mink[0].y);
	bSeg[15].SetSeg(mink[11].x, mink[11].y, mink[1].x, mink[1].y);

	//実はここら辺イラン気がする
	for (int i = 0; i < 16; i++)
		if (ColSegment(aSeg, bSeg[i], &Ttmp))
			if (Ttmp < T1)
			{
				//法線ベクトル
				normal = Normalize(XMFLOAT2(-bSeg[i].v.y, bSeg[i].v.x));
				if (VecDot(aSeg.v, normal) > 0)
				{
					T1 = Ttmp;
					counter = i;
				}
			}

	if (counter != 20)
	{
		//aの移動ベクトルを内分比でうんぬん→はみ出した部分のベクトル
		aVec_2 = XMFLOAT2((1 - T1)*aVec.x, (1 - T1)*aVec.y);

		//法線ベクトル
		normal = Normalize(XMFLOAT2(-bSeg[counter].v.y, bSeg[counter].v.x));

		//正規化したbSeg.v
		normalizedB = Normalize(bSeg[counter].v);

		//Dot
		Dot = VecDot(aVec_2, normalizedB);

		if(end)
		fixVec = XMFLOAT2((-aVec_2.x*1.01f) + Dot * normalizedB.x,
			(-aVec_2.y*1.01f) + Dot * normalizedB.y);
		else
			fixVec = XMFLOAT2((-aVec_2.x*1.01f),
			(-aVec_2.y*1.01f));
		a.parent->SetPos(XMFLOAT2(a.parent->GetPos().x + fixVec.x,
			a.parent->GetPos().y + fixVec.y));

		a.parent->SetSpeed(XMFLOAT2(Dot * normalizedB.x, Dot * normalizedB.y));
		return 1;

	}
	return 0;
};


//ヒットボックス同士の最短の交差判定距離とその線分
//速度もいるわ
float Search_path(Hitbox a, Hitbox b) {
	//相対移動ベクトルを求める
	XMFLOAT2 aVec = a.parent->GetMove(b.parent->GetMove());

	//aの位置　ｂの位置//ここら辺クラスで保持してたほうがええかも
	XMFLOAT2 apos = a.parent->GetPos();
	XMFLOAT2 bpos = b.parent->GetPos();

	//aの相対移動ベクトルと始点のセグメントを作る
	Segment aSeg;
	aSeg.SetSeg(apos.x - aVec.x, apos.y - aVec.y, apos.x, apos.y);
	//bのミンコフスキー差のセグメントを作る
	Segment bSeg[16];

	//左上から時計回りに
	XMFLOAT2 mink[12];

	mink[0] = XMFLOAT2(bpos.x + b.localpos[0].x - a.localpos[1].x, bpos.y + b.localpos[0].y - a.localpos[1].y);
	mink[1] = XMFLOAT2(bpos.x + b.localpos[0].x - a.localpos[3].x, bpos.y + b.localpos[0].y - a.localpos[3].y);
	mink[2] = XMFLOAT2(bpos.x + b.localpos[0].x - a.localpos[2].x, bpos.y + b.localpos[0].y - a.localpos[2].y);
	mink[3] = XMFLOAT2(bpos.x + b.localpos[1].x - a.localpos[3].x, bpos.y + b.localpos[1].y - a.localpos[3].y);
	mink[4] = XMFLOAT2(bpos.x + b.localpos[1].x - a.localpos[2].x, bpos.y + b.localpos[1].y - a.localpos[2].y);
	mink[5] = XMFLOAT2(bpos.x + b.localpos[1].x - a.localpos[0].x, bpos.y + b.localpos[1].y - a.localpos[0].y);
	mink[6] = XMFLOAT2(bpos.x + b.localpos[3].x - a.localpos[2].x, bpos.y + b.localpos[3].y - a.localpos[2].y);
	mink[7] = XMFLOAT2(bpos.x + b.localpos[3].x - a.localpos[0].x, bpos.y + b.localpos[3].y - a.localpos[0].y);
	mink[8] = XMFLOAT2(bpos.x + b.localpos[3].x - a.localpos[1].x, bpos.y + b.localpos[3].y - a.localpos[1].y);
	mink[9] = XMFLOAT2(bpos.x + b.localpos[2].x - a.localpos[0].x, bpos.y + b.localpos[2].y - a.localpos[0].y);
	mink[10] = XMFLOAT2(bpos.x + b.localpos[2].x - a.localpos[1].x, bpos.y + b.localpos[2].y - a.localpos[1].y);
	mink[11] = XMFLOAT2(bpos.x + b.localpos[2].x - a.localpos[3].x, bpos.y + b.localpos[2].y - a.localpos[3].y);


	//交差してたら0~1になるはず
	float T1 = 2;
	float Ttmp = 2;

	//
	int counter = 20;
	XMFLOAT2 aVec_2;

	//法線ベクトル
	XMFLOAT2 normal;

	//衝突後に修正するベクトル
	XMFLOAT2 fixVec;

	//aVec_2とbSegの内積
	float Dot;
	bSeg[0].SetSeg(mink[0].x, mink[0].y, mink[1].x, mink[1].y);
	bSeg[1].SetSeg(mink[1].x, mink[1].y, mink[2].x, mink[2].y);
	bSeg[2].SetSeg(mink[1].x, mink[1].y, mink[3].x, mink[3].y);
	bSeg[3].SetSeg(mink[2].x, mink[2].y, mink[4].x, mink[4].y);
	bSeg[4].SetSeg(mink[3].x, mink[3].y, mink[4].x, mink[4].y);
	bSeg[5].SetSeg(mink[4].x, mink[4].y, mink[5].x, mink[5].y);
	bSeg[6].SetSeg(mink[4].x, mink[4].y, mink[6].x, mink[6].y);
	bSeg[7].SetSeg(mink[5].x, mink[5].y, mink[7].x, mink[7].y);
	bSeg[8].SetSeg(mink[6].x, mink[6].y, mink[7].x, mink[7].y);
	bSeg[9].SetSeg(mink[7].x, mink[7].y, mink[8].x, mink[8].y);
	bSeg[10].SetSeg(mink[7].x, mink[7].y, mink[9].x, mink[9].y);
	bSeg[11].SetSeg(mink[8].x, mink[8].y, mink[10].x, mink[10].y);
	bSeg[12].SetSeg(mink[9].x, mink[9].y, mink[10].x, mink[10].y);
	bSeg[13].SetSeg(mink[10].x, mink[10].y, mink[11].x, mink[11].y);
	bSeg[14].SetSeg(mink[10].x, mink[10].y, mink[0].x, mink[0].y);
	bSeg[15].SetSeg(mink[11].x, mink[11].y, mink[1].x, mink[1].y);

	//実はここら辺イラン気がする
	for (int i = 0; i < 16; i++)
		if (ColSegment(aSeg, bSeg[i], &Ttmp))
			if (Ttmp < T1)
			{
				//法線ベクトル
				normal = Normalize(XMFLOAT2(-bSeg[i].v.y, bSeg[i].v.x));
				if (VecDot(aSeg.v, normal) > 0)
				{
					T1 = Ttmp;
					counter = i;
				}
			}

	return T1;
};

void Collition() {
	extern std::vector<std::vector<Hitbox*>>g_ActiveHitboxList;
	extern std::vector<std::vector<Hitbox*>>g_PassiveHitboxList;
	float T1 = 2.0f;
	float tmp;

	//最短で交差するベクトルを受け取る
	Segment* Seg = nullptr;
	//最短で交差するひっとぼっくす
	Hitbox* paBox = nullptr;
	Hitbox* pbBox = nullptr;

	int cnt;
	//当たり判定処理　
	for (auto ActiveHitboxList : g_ActiveHitboxList)
	{
		cnt = 0;
		for (int i = 0; i < 3; i++)
		{
			T1 = 2.0f;
			for (auto active : ActiveHitboxList)//アクティブヒットボックスリストの中のヒットボックスを一つずつ
			{
				//受け身のヒットボックスを走査
				for (auto PassiveHitboxList : g_PassiveHitboxList)
					for (auto passive : PassiveHitboxList)
					{
						//そのヒットボックスの最短の衝突判定を見つける
						tmp = Search_path(*active, *passive);
						if (T1 > tmp)
						{
							T1 = tmp;
							paBox = active;
							pbBox = passive;
						}
					}
			}

			cnt++;
			if (T1 < 1.1f)//交差してるやつあったら衝突させる
			{
				if (cnt < 3)
				{
					check_Hit(*paBox, *pbBox, 1);
				}
				else
				{
					check_Hit(*paBox, *pbBox, 0);
				}
			}
			else
			{
				break;
			}
		}
	}

}