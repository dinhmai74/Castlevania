#include "ItemFactory.h"

ItemFactory* ItemFactory::instance = nullptr;

Item* ItemFactory::getItem(int type, D3DXVECTOR2 pos, bool isEnable)
{
	Item* item;
	auto vx = 0.0f, vy = 0.0f;
	auto gravity = 0.0015f;
	switch (type)
	{
	case itemSmallHeart:
		item = new ItemHeart();
		gravity = 0.0001f;
		vx = 0.05f;
		break;
	case itemWhip: item = new ItemWhip(); break;
	case itemDagger: item = new ItemDagger(); break;
	case itemAxe: item = new ItemDagger(); break;
	case itemBigHeart: item = new ItemHeart(itemBigHeart); break;
	default: item = new ItemHeart();
	}

	item->setEnable(isEnable);
	item->setGravity(gravity);
	item->setSpeed(vx, vy);
	item->setPosition(pos.x, pos.y);
	item->setInitPos(pos);
	return item;
}