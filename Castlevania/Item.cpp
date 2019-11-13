#include "Item.h"

Item::Item()
{
	init();
}

Item::Item(int type) {
	init();
	setItemType(type);
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
	updateGravity(dt,gravity);
}

void Item::setItemType(int type)
{
	itemType= type;
	setAnimId(type);
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
	return GameObject::getBoundingBoxBaseOnFile();
}


void Item::initAnim()
{
	addAnimation(itemSmallHeart, "item_smallheart_ani");
	addAnimation(itemBigHeart, "item_largeheart_ani");
	addAnimation(itemWhip, "item_chain_ani");
	addAnimation(itemDagger, "item_dagger_ani");
	addAnimation(itemAxe, "item_axe_ani");
	addAnimation(itemStopWatch, "item_stopwatch_ani");
	addAnimation(itemBoomerang, "item_boomerang_ani");
	addAnimation(itemBlueMoneyBag, "item_bluemoneybag_ani");
	addAnimation(itemWhiteMoneyBag, "item_whitemoneybag_ani");
	addAnimation(itemRedMoneyBag, "item_redmoneybag_ani");
	addAnimation(itemPorkChop, "item_porkchop_ani");
	addAnimation(itemHolyCross, "item_cross_ani");
}

void Item::init()
{
	type = OBItem;
	setFaceSide(SideRight);
	initAnim();
	animId = itemBigHeart;
	appearTimer = new Timer(TIMER_ITEM_DURATION);
	untouchableTimer = new Timer(TIMER_ITEM_DURATION - TIMER_ITEM_UNTOUCHABLE_DURATION);
	appearTimer->start();
	untouchableTimer->start();
}
