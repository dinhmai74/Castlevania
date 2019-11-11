
#pragma once
#include "Item.h"

class ItemMoneyBag: public Item
{
public:
	ItemMoneyBag();
	ItemMoneyBag(int type, int score=700);
	~ItemMoneyBag();

	int getScore() const { return score; }
	void setScore(int val) { score = val; }
private:
	int score;
};
