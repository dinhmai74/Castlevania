#pragma once
#include "Item.h"

class ItemWhip :
	public Item
{
public:
	ItemWhip();
	~ItemWhip() = default;

};

inline ItemWhip::ItemWhip()
{
	itemType = itemWhip;
	gravity = 0.001;
	animationId = itemWhip;
}

