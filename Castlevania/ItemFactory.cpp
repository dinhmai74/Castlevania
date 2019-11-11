#include "ItemFactory.h"
#include "ItemMoneyBag.h"

ItemFactory* ItemFactory::instance = nullptr;

Item* ItemFactory::getItem(int type, D3DXVECTOR2 pos, bool isEnable) {
	Item* item;
	auto vx = 0.0f, vy = 0.0f;
	auto gravity = 0.0015f;
	switch (type) {
	case itemSmallHeart:
		item = new ItemHeart();
		gravity = 0.0001f;
		vx = 0.05f;
		break;
	case itemBigHeart: item = new ItemHeart(itemBigHeart); break;
	case itemBlueMoneyBag:
		item = new ItemMoneyBag(type, 700);
		break;
	case itemWhiteMoneyBag:
		item = new ItemMoneyBag(type, 500);
		break;
	case itemRedMoneyBag:
		item = new ItemMoneyBag(type, 1000);
		break;
	default:
		item = new Item(type);
	}

	item->setEnable(isEnable);
	item->setGravity(gravity);
	item->setSpeed(vx, vy);
	item->setPos(pos.x, pos.y);
	item->setInitPos(pos);
	return item;
}