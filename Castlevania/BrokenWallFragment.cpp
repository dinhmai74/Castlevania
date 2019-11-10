#include "BrokenWallFragment.h"


BrokenWallFragment::BrokenWallFragment() {
	initAnim();
	setState(idle);
	setType(OBBrokenWallFragment);
	gravity = 0.0008f;
	currentFrame = 0;
	animId = idle;
}

BrokenWallFragment::~BrokenWallFragment() = default;

void BrokenWallFragment::initAnim() {
	addAnimation(idle, "broken_wall_fragment_ani");
}

void BrokenWallFragment::update(DWORD dt, vector<GameObject*>* coObjects) {
	GameObject::update(dt);
	animId = state;
	x += dx;
	y += dy;
	updateGravity(dt);
}
