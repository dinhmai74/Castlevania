#include "ItemMoneyBag.h"
#include <string> 


ItemMoneyBag::ItemMoneyBag() {
	initBag();
	setItemType(itemBlueMoneyBag);
	setScore(100);
}

ItemMoneyBag::ItemMoneyBag(int type, int score) {
	initBag();
	setItemType(type);
	setScore(score);
}
ItemMoneyBag::~ItemMoneyBag() {}

void ItemMoneyBag::setCollected(bool val /*= true*/)
{
	timerShowScore->startDeep();
	setEnable(false);
}

void ItemMoneyBag::update(DWORD dt, vector<GameObject*>* boundary)
{
	if (timerShowScore->isRunning()) {
		faceSide = -1;
		animId = score;
		return;
	}
	if (timerShowScore->isTimeUpAndRunAlr()) {
		setDisable();
		return;
	}

	Item::update(dt, boundary);
}

Box ItemMoneyBag::getBoundingBox()
{
	if (timerShowScore->isRunning()) return{ 0,0,0,0 };

	return Item::getBoundingBox();
}

void ItemMoneyBag::render()
{
	animations[animId]->render(faceSide, x, y);
}

void ItemMoneyBag::initBag()
{
	addAnimation(1000, "score_1000_ani");
	addAnimation(700, "score_700_ani");
	addAnimation(400, "score_400_ani");
	addAnimation(100, "score_100_ani");
}
