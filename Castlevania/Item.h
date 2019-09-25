#pragma once
#include "GameObject.h"
#include "Constants.h"

class Item :
	public GameObject
{
protected:
	int itemType;
public:
	void init();
	Item();
	~Item();

	void update(DWORD dt, vector<GameObject*>* boundary);
	void setItemType(const int type);
	int getItemType() const { return itemType; }
	Box getBoundingBox() override;
private:
	void initAnim() override;
	void checkCollision(DWORD dt, vector<GameObject*>* boundary);
};
inline void Item::initAnim()
{
	addAnimation(itemSmallHeart, "item_smallheart_ani");
	addAnimation(itemBigHeart, "item_largeheart_ani");
	addAnimation(itemWhip, "item_chain_ani");
	addAnimation(itemDagger, "item_dagger_ani");
	addAnimation(itemAxe, "item_axe_ani");
	addAnimation(itemBumerang, "item_cross_ani");
	addAnimation(itemBlueMoneyBag, "item_blue_ani");
	addAnimation(itemWhiteMoneyBag, "item_white_ani");
	addAnimation(itemRedMoneyBag, "item_red_ani");
}

inline void Item::init()
{
	type = item;
	setFaceSide(FaceSide::right);
	initAnim();
}

inline Item::Item()
{
	init();
}

inline Item::~Item()
= default;

inline void Item::checkCollision(DWORD dt, vector<GameObject*>* boundary)
{
	GameObject::checkCollisionAndStopMovement(dt, boundary);
}

inline void Item::update(DWORD dt, vector<GameObject*>* boundary)
{
	GameObject::update(dt);
	checkCollision(dt, boundary);
	updateGravity(gravity);
}


inline void Item::setItemType(const int type)
{
	itemType = type;
	animId = itemType;
}

inline Box Item::getBoundingBox()
{
	return GameObject::getBoundingBox(-1, -1);
}
