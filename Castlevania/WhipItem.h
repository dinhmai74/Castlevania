#pragma once
#include "Item.h"

class WhipItem :
	public Item
{
public:
	WhipItem();
	~WhipItem() = default;

};

inline WhipItem::WhipItem()
{
	itemType = itemWhip;
	gravity = 0.001f;
	animationId = itemWhip;
}

