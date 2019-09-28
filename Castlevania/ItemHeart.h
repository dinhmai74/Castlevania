#pragma once
#include "Item.h"
#include "Territory.h"

class ItemHeart final : public Item
{
	int energy;
public:
	ItemHeart();
	explicit ItemHeart(int type);
	~ItemHeart();
	int getEnergy() const;

	void update(DWORD dt, vector<GameObject*>* boundary) override;
	void checkOutOfBoundary();

	virtual void checkCollision(DWORD dt, vector<GameObject*>* boundary) override;

};
