#pragma once
#include"quad.h"

#include"tex.h"
#include "Object.h"
class Scean :public Object
{
private:
	enum SCEANTYPE {
		TITLE = 0,
		SENTAKU,
		SENTAKU2,
	};

public:
	Scean();
	~Scean();
};

