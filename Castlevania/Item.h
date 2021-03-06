#pragma once
#include "GameObject.h"
#include "Constants.h"

enum ItemType
{
	itemDagger,
	itemAxe,
	itemBoomerang,
	itemHolyWater,
	itemStopWatch,
	itemWhip = 102,
	itemBigHeart = 100,
	itemSmallHeart = 101,
	itemHolyWaterEffect = 103,
	itemRedMoneyBag,
	itemWhiteMoneyBag,
	itemBlueMoneyBag,
	itemPorkChop,
	itemHolyCross,
	itemDoubleShot,
	itemGoldPotion,
	itemTripleShot,
};

class Item :
	public GameObject
{
protected:
	int itemType;
	bool isCollideGround;
	void checkDurationToDisapear();
	Timer* appearTimer;
	Timer* untouchableTimer;
public:
	void init();
	Item();
	explicit Item(int type);
	~Item();

	void update(DWORD dt, vector<GameObject*>* boundary) override;
	void setItemType(int type);
	int getItemType() const;
	virtual void checkCollision(DWORD dt, vector<GameObject*>* boundary);
	virtual void setCollected(bool val = true) ;
private:
	void initAnim() override;
};
