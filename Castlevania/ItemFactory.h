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

	static Item* getItem(int type)
	{
		switch (type)
		{
		case itemSmallHeart: return new ItemHeart();
		case itemWhip: return new ItemWhip();
		default: ;
		}
		return nullptr;
	}

private:
	ItemFactory() = default;
	static ItemFactory* instance;
};


ItemFactory* ItemFactory::instance = nullptr;
