#include "Item.h"

Item::Item()
{
	init();
}

Item::~Item()
= default;

void Item::checkCollision(DWORD dt, vector<GameObject*>* boundary)
{
	checkCollisionAndStopMovement(dt, boundary);
}

void Item::update(DWORD dt, vector<GameObject*>* boundary)
{
	GameObject::update(dt);
	checkDurationToDisapear();
	checkCollision(dt, boundary);
	updateGravity(gravity);
}

void Item::setItemType(const int type)
{
	itemType = type;
	animId = itemType;
}

int Item::getItemType() const
{
	return itemType;
}

void Item::checkDurationToDisapear()
{
	if (untouchableTimer->isTimeUpAndRunAlr()) alpha = rand() % 255;
	if (appearTimer->isTimeUpAndRunAlr()) setDisable();
}

Box Item::getBoundingBox()
{
	return GameObject::getBoundingBox(-1, -1);
}


void Item::initAnim()
{
	addAnimation(itemSmallHeart, "item_smallheart_ani");
	addAnimation(itemBigHeart, "item_largeheart_ani");
	addAnimation(itemWhip, "item_chain_ani");
	addAnimation(itemDagger, "item_dagger_ani");
	addAnimation(itemAxe, "item_axe_ani");
	addAnimation(itemBoomerang, "item_cross_ani");
	addAnimation(itemBlueMoneyBag, "item_blue_ani");
	addAnimation(itemWhiteMoneyBag, "item_white_ani");
	addAnimation(itemRedMoneyBag, "item_red_ani");
}

void Item::init()
{
	type = item;
	setFaceSide(SideRight);
	initAnim();
	appearTimer = new Timer(TIMER_ITEM_DURATION);
	untouchableTimer = new Timer(TIMER_ITEM_DURATION - TIMER_ITEM_UNTOUCHABLE_DURATION);
	appearTimer->start();
	untouchableTimer->start();
}
