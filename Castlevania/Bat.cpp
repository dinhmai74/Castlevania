#include "Bat.h"



Bat::Bat()
{
	Bat::initAnim();
	setFaceSide(1);
	state = idle;
	animId = idle;
	setType(OBDecorationBat);

	vx = (float)(-50 + rand() % 100) / 1000;
	vy = (float)(-50 + rand() % 100) / 1000;

	x = 100 + rand() % 300;
	y = 50 + rand() % 100;
}

Bat::~Bat()
{
}

void Bat::update(DWORD dt, vector<GameObject*>* coObjects) {
	this->dt = dt;
	dx = vx * dt;
	dy = vy * dt;
	x += dx;
	y += dy;
}
