
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

	virtual void setCollected(bool val = true) override;

	virtual void update(DWORD dt, vector<GameObject*>* boundary) override;


	virtual Box getBoundingBox() override;


	virtual void render() override;

private:
	int score;
	Timer* timerShowScore= new Timer(500);

	void initBag();
};
