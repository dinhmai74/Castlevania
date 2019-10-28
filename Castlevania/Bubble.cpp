#include "Bubble.h"

Bubble::Bubble() {
	initAnim();
	setState(idle);
	setType(OBBubble);
	gravity = 0.0008f;
	currentFrame = 0;
	animId = idle;
}
Bubble::~Bubble() = default;

void Bubble::update(DWORD dt, vector<GameObject*> * coObjects) {
	GameObject::update(dt);
	animId = state;
	x += dx;
	y += dy;
	updateGravity(dt);
}


void Bubble::initAnim() {
	addAnimation(idle, "bubble_idle_ani");
}
