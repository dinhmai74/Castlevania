#include "Helicopter.h"


Helicopter::Helicopter() {
	Helicopter::initAnim();
	setFaceSide(1);
	setAnimId(idle);
	setType(OBHelicopter);
	state = idle;
	vx = -0.013f;
	vy = -0.001f;
}

Helicopter::~Helicopter() {}
void Helicopter::initAnim() {
	addAnimation(idle, "helicopter_idle_ani");
}

void Helicopter::update(DWORD dt, vector<GameObject*>* coObjects) {
	x += vx*dt;
	y += vy*dt;
}
