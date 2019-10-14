#pragma once
#include "GameObject.h"
#include "Constants.h"

enum ItemType
{
	itemDagger,
	itemAxe,
	itemBoomerang,
	itemHolyWater,
	itemRedMoneyBag,
	itemWhiteMoneyBag,
	itemBlueMoneyBag,
	itemWhip = 102,
	itemBigHeart = 100,
	itemSmallHeart = 101,
	itemHolyWaterEffect = 103,
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
	~Item();

	void update(DWORD dt, vector<GameObject*>* boundary) override;
	void setItemType(const int type);
	int getItemType() const;
	Box getBoundingBox() override;
	virtual void checkCollision(DWORD dt, vector<GameObject*>* boundary);
private:
	void initAnim() override;
};
