#pragma once
#include "Item.h"

class ItemDagger : public Item
{
public:
	ItemDagger();
	~ItemDagger() = default;
};

inline ItemDagger::ItemDagger()
{
	itemType = itemDagger;
	gravity = 0.0015;
	animationId = itemDagger;
}
