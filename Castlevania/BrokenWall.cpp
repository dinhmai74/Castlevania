#include "BrokenWall.h"
#include "StageManager.h"
#include "ItemFactory.h"


BrokenWall::BrokenWall() {
	init(x, y);
}

void BrokenWall::init(float x, float y) {
	initAnim();
	setType(OBBrokenWall);
	setPos(x, y);
	setInitPos({ x, y });
	auto vx = 0.06;
	auto fragment = new BrokenWallFragment();
	fragment->setPos(x, y);
	fragment->setSpeed(vx, -0.25);

	auto fragmentMiddle = new BrokenWallFragment();
	fragmentMiddle->setPos(x, y);
	fragmentMiddle->setSpeed(vx * 0.3, -0.25);

	auto fragmentRight = new BrokenWallFragment();
	fragmentRight->setPos(x, y);
	fragmentRight->setSpeed(-vx * 1.5, -0.25);
	fragments.push_back(fragment);
	fragments.push_back(fragmentMiddle);
	fragments.push_back(fragmentRight);

	timerActive->stop();
	setState(Init);
	setAnimId(Init);
	setItemNx(1);

	setType(OBBrokenWall);
	setBoundaryType(BoundaryBrokenWall);
}

BrokenWall::BrokenWall(float x, float y) {
	init(x, y);
}


BrokenWall::~BrokenWall() {}

void BrokenWall::initAnim() {
	addAnimation(Init, "empty_ani");
	addAnimation(Broking, "broken_wall_default_ani");
	addAnimation(Broken, "broken_wall_default_ani");
}


void BrokenWall::render() {
	if (state == Broking) for (auto fragMent : fragments) if (fragMent)fragMent->render();

	animations[animId]->render(1, x, y);
	ItemContainer::render();
}

void BrokenWall::update(DWORD dt, vector<GameObject*>* coObjects) {
	ItemContainer::update(dt);
	animId = state;
	if (state == Broking) {
		for (auto fragMent : fragments) fragMent->update(dt, coObjects);
	}

	if (timerActive->isTimeUpAndRunAlr()) {
		setState(Broken);
	}
}

void BrokenWall::updateAnimId() {
	setAnimId(state);
	ItemContainer::updateAnimId();
}

bool BrokenWall::getHurt(int nx, int ny, int hpLose) {
	if (hpLose > 0 && state == Init) {
		setState(Broking);
		generateItem();
		timerActive->start();
		return true;
	}
	return false;
}

Box BrokenWall::getBoundingBox() {
	if (state == Init)return getBoundingBoxBaseOnFile();

	return { 0,0,0,0 };
}

