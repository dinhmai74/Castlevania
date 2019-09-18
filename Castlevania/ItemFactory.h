#include "Item.h"
#include "ItemHeart.h"
#include "ItemWhip.h"

class ItemFactory
{
public:
	~ItemFactory() { instance = nullptr; }

	static ItemFactory* Get()
	{
		if (instance == nullptr)
			instance = new ItemFactory;
		return instance;
	}

	static Item* getItem(int type,bool isEnable=true)
	{
		Item* item;
		switch (type)
		{
		case itemSmallHeart: item = new ItemHeart(); break;
		case itemWhip: item = new ItemWhip(); break;
		default: item = new Item();
		}

		item->setEnable(isEnable);
		return item;
	}

private:
	ItemFactory() = default;
	static ItemFactory* instance;
};


ItemFactory* ItemFactory::instance = nullptr;
