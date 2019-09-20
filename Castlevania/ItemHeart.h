#pragma once
#include "Item.h"

class ItemHeart final : public Item
{
public:
	ItemHeart();
	~ItemHeart() = default;
};

inline ItemHeart::ItemHeart()
{
	itemType = itemSmallHeart;
	gravity = 0.0002;
	animId = itemSmallHeart;
}
