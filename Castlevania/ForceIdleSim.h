#pragma once
#include "Boundary.h"

class ForceIdleSim : public Boundary
{
public:
	ForceIdleSim();
	~ForceIdleSim();
	int getDirection() const { return direction; }
	void setDirection(int val) { direction = val; }
	int getNextX() const { return nextX; }
	void setNextX(int val) { nextX = val; }
	float getNextY() const { return nextY; }
	void setNextY(float val) { nextY = val; }
private:
	int direction;
	float nextX;
	float nextY;
};

