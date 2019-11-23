#pragma once
#include "Boundary.h"

enum StairType
{
	StairStartUp,
	StairMiddle,
	StairEnd,
	StairStartDown,
};

class Stair: public Boundary
{
public:
	Stair();
	~Stair();
	int getStairType() const { return stairType; }
	void setStairType(int val) { stairType = val; }
	D3DXVECTOR2 getNextPos() const { return nextPos; }
	void setNextPos(D3DXVECTOR2 val) { nextPos = val; }
	float getInitStairPos() const { return initStairPos; }
	void setInitStairPos(float val) { initStairPos = val; }
private:
	int stairType;
	float initStairPos;
	D3DXVECTOR2 nextPos;
};
