#include "Bubbles.h"


void Bubbles::init(float x, float y) {
	initAnim();
	float vx = (float)(-100 + rand() % 200) / 1000;
	setInitPos({ x,y });
	setPosition(x, y);
	auto bubble = new Bubble();
	bubble->setPosition(x, y);
	bubble->setSpeed(vx, -0.25);

	auto bubbleL = new Bubble();
	bubbleL->setPosition(x, y);
	bubbleL->setSpeed(vx, 0);

	auto bubbleR = new Bubble();
	bubbleR->setPosition(x, y);
	bubbleR->setSpeed(-vx, 0);
	bubbles.push_back(bubble);
	bubbles.push_back(bubbleL);
	bubbles.push_back(bubbleR);

	setType(OBBubbles);
	timerActive->start();
}

Bubbles::Bubbles() {
	init(20, 100);
}

Bubbles::Bubbles(float x, float y) {
	init(x, y);
}

Bubbles::~Bubbles() = default;

void Bubbles::initAnim() {
	addAnimation(idle, "bubble_idle_ani");
	animId = idle;
}

void Bubbles::render() {
	for (auto bubble : bubbles) bubble->render();
}

void Bubbles::update(DWORD dt, vector<GameObject*>* coObjects /*= nullptr*/) {
	if (timerActive->isTimeUpAndRunAlr()) setActive(false);
	for (auto bubble : bubbles) bubble->update(dt,coObjects);
}
