#include "ItemMoneyBag.h"


ItemMoneyBag::ItemMoneyBag() {
	setItemType(itemBlueMoneyBag);
}

ItemMoneyBag::ItemMoneyBag(int type, int score) {
	setItemType(type);
	setScore(score);
}
ItemMoneyBag::~ItemMoneyBag() {}
