#pragma once
#include "Boundary.h"

enum StairType
{
	StairStart,
	StairMiddle,
	StairEnd
};

class Stair: public Boundary
{
public:
	D3DXVECTOR2 getCanStandPos();
	Stair();
	~Stair();
	int getStairType() const { return stairType; }
	void setStairType(int val) { stairType = val; }
	D3DXVECTOR2 getNextPos() const { return nextPos; }
	void setNextPos(D3DXVECTOR2 val) { nextPos = val; }
private:
	int stairType;
	D3DXVECTOR2 nextPos;
};
