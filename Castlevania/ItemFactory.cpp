#include "ItemFactory.h"


ItemFactory* ItemFactory::instance = nullptr;

Item* ItemFactory::getItem(int type, D3DXVECTOR2 pos, bool isEnable)
{
	Item* item;
	switch (type)
	{
		case itemSmallHeart: item = new ItemHeart();break;
		case itemWhip: item = new ItemWhip();break;
		case itemDagger: item = new ItemDagger();break;
		case itemAxe: item = new ItemDagger();break;
		case itemBigHeart: item = new ItemHeart(itemBigHeart);break;
		default: item = new ItemHeart();
	}

	item->setEnable(isEnable);
	item->setPosition(pos.x, pos.y);
	item->setInitPos(pos);
	return item;
}
