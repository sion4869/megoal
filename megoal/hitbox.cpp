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

//dir 0:�E�@1:��
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
//����
float VecDot(XMFLOAT2 v1, XMFLOAT2 v2) {
	return v1.x * v2.x + v1.y *  v2.y;
}
//�O��
float VecCross(XMFLOAT2* v1, XMFLOAT2* v2) {
	return v1->x * v2->y - v1->y * v2->x;
}
float VecCross(XMFLOAT2 v1, XMFLOAT2 v2) {
	return v1.x * v2.y - v1.y * v2.x;
}

//��������
bool ColSegment(
	Segment seg1,          // ����1
	Segment seg2,          // ����2
	float* outT1,       // ����1�̓�����i�o�́j
	float* outT2        // ����2�̓�����i�o��)
) {
	XMFLOAT2 v;
	v.x = seg2.s.x - seg1.s.x;
	v.y = seg2.s.y - seg1.s.y;
	float Crs_v1_v2 = VecCross(&seg1.v, &seg2.v);
	if (Crs_v1_v2 == 0.0f)
	{
		//���s���
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
		//�������Ă��Ȃ�
		return false;
	}
	return true;
}

bool check_Hit(Hitbox a, Hitbox b, bool end) {
	//���Έړ��x�N�g�������߂�
	XMFLOAT2 aVec = a.parent->GetMove(b.parent->GetMove());

	//a�̈ʒu�@���̈ʒu//������ӃN���X�ŕێ����Ă��ق�����������
	XMFLOAT2 apos = a.parent->GetPos();
	XMFLOAT2 bpos = b.parent->GetPos();

	//a�̑��Έړ��x�N�g���Ǝn�_�̃Z�O�����g�����
	Segment aSeg;
	aSeg.SetSeg(apos.x - aVec.x, apos.y - aVec.y, apos.x, apos.y);
	//b�̃~���R�t�X�L�[���̃Z�O�����g�����
	Segment bSeg[16];

	//���ォ�玞�v����
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


	//�������Ă���0~1�ɂȂ�͂�
	float T1 = 2;
	float Ttmp = 2;

	//
	int counter = 20;
	XMFLOAT2 aVec_2;

	//�@���x�N�g��
	XMFLOAT2 normal;

	//���K������bSeg.v
	XMFLOAT2 normalizedB;

	//�Փˌ�ɏC������x�N�g��
	XMFLOAT2 fixVec;

	//aVec_2��bSeg�̓���
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

	//���͂�����ӃC�����C������
	for (int i = 0; i < 16; i++)
		if (ColSegment(aSeg, bSeg[i], &Ttmp))
			if (Ttmp < T1)
			{
				//�@���x�N�g��
				normal = Normalize(XMFLOAT2(-bSeg[i].v.y, bSeg[i].v.x));
				if (VecDot(aSeg.v, normal) > 0)
				{
					T1 = Ttmp;
					counter = i;
				}
			}

	if (counter != 20)
	{
		//a�̈ړ��x�N�g���������ł���ʂ񁨂͂ݏo���������̃x�N�g��
		aVec_2 = XMFLOAT2((1 - T1)*aVec.x, (1 - T1)*aVec.y);

		//�@���x�N�g��
		normal = Normalize(XMFLOAT2(-bSeg[counter].v.y, bSeg[counter].v.x));

		//���K������bSeg.v
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


//�q�b�g�{�b�N�X���m�̍ŒZ�̌������苗���Ƃ��̐���
//���x�������
float Search_path(Hitbox a, Hitbox b) {
	//���Έړ��x�N�g�������߂�
	XMFLOAT2 aVec = a.parent->GetMove(b.parent->GetMove());

	//a�̈ʒu�@���̈ʒu//������ӃN���X�ŕێ����Ă��ق�����������
	XMFLOAT2 apos = a.parent->GetPos();
	XMFLOAT2 bpos = b.parent->GetPos();

	//a�̑��Έړ��x�N�g���Ǝn�_�̃Z�O�����g�����
	Segment aSeg;
	aSeg.SetSeg(apos.x - aVec.x, apos.y - aVec.y, apos.x, apos.y);
	//b�̃~���R�t�X�L�[���̃Z�O�����g�����
	Segment bSeg[16];

	//���ォ�玞�v����
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


	//�������Ă���0~1�ɂȂ�͂�
	float T1 = 2;
	float Ttmp = 2;

	//
	int counter = 20;
	XMFLOAT2 aVec_2;

	//�@���x�N�g��
	XMFLOAT2 normal;

	//�Փˌ�ɏC������x�N�g��
	XMFLOAT2 fixVec;

	//aVec_2��bSeg�̓���
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

	//���͂�����ӃC�����C������
	for (int i = 0; i < 16; i++)
		if (ColSegment(aSeg, bSeg[i], &Ttmp))
			if (Ttmp < T1)
			{
				//�@���x�N�g��
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

	//�ŒZ�Ō�������x�N�g�����󂯎��
	Segment* Seg = nullptr;
	//�ŒZ�Ō�������Ђ��Ƃڂ�����
	Hitbox* paBox = nullptr;
	Hitbox* pbBox = nullptr;

	int cnt;
	//�����蔻�菈���@
	for (auto ActiveHitboxList : g_ActiveHitboxList)
	{
		cnt = 0;
		for (int i = 0; i < 3; i++)
		{
			T1 = 2.0f;
			for (auto active : ActiveHitboxList)//�A�N�e�B�u�q�b�g�{�b�N�X���X�g�̒��̃q�b�g�{�b�N�X�������
			{
				//�󂯐g�̃q�b�g�{�b�N�X�𑖍�
				for (auto PassiveHitboxList : g_PassiveHitboxList)
					for (auto passive : PassiveHitboxList)
					{
						//���̃q�b�g�{�b�N�X�̍ŒZ�̏Փ˔����������
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
			if (T1 < 1.1f)//�������Ă���������Փ˂�����
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