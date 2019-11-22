#include "DecorationHelicopter.h"


DecorationHelicopter::DecorationHelicopter() {
	initAnim();
	setFaceSide(1);
	setAnimId(idle);
	setType(OBHelicopter);
	state = idle;
	vx = -0.013f;
	vy = -0.001f;

}
DecorationHelicopter::~DecorationHelicopter() {}
void DecorationHelicopter::initAnim() {
	addAnimation(idle, "helicopter_idle_ani");
}

void DecorationHelicopter::update(DWORD dt, vector<GameObject*>* coObjects) {
	x += vx * dt;
	y += vy * dt;
}
