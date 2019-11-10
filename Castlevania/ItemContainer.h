#pragma once
#include "Boundary.h"

class ItemContainer :
	public Boundary{
public:
	void generateItem();
	ItemContainer();
	~ItemContainer() = default;
	int getItemNx() const { return itemNx; }
	void setItemNx(int val) { itemNx = val; }
	int getItemId() const { return itemId; }
	void setItemId(int val) { itemId = val; }
private:
	bool isGeneratedItem;
	int itemNx;
	int itemId;
};

