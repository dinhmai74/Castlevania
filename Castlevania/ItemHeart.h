#pragma once
#include "Item.h"

class ItemHeart final : public Item
{
	int energy;
public:
	ItemHeart();
	explicit ItemHeart(int type);
	~ItemHeart() = default;
	int getEnergy() const { return energy; }
};

inline ItemHeart::ItemHeart()
{
	itemType = itemSmallHeart;
	gravity = 0.0002;
	animId = itemSmallHeart;
	energy = 1;
}

inline ItemHeart::ItemHeart(int type)
{
	itemType = type;
	gravity = 0.0002;
	energy = 5;
	animId = type;
}
