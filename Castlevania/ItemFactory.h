#pragma once
#include "Item.h"
#include "ItemHeart.h"

class ItemFactory
{
public:
	~ItemFactory() { instance = nullptr; }

	static ItemFactory* Get()
	{
		if (instance == nullptr)
			instance = new ItemFactory();
		return instance;
	}

	static Item* getItem(int type, D3DXVECTOR2 pos, bool isEnable = true);

private:
	ItemFactory() = default;
	static ItemFactory* instance;
};
