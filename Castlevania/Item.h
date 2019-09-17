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
	void render() override;
	Box getBoundingBox() override;
private:
	void initAnim() override;
	void checkCollision(DWORD dt, vector<GameObject*>* boundary);
};

inline void Item::init()
{
	itemType = heartItem;
	animationId = itemType;
	faceSide = FaceSide::right;
	gravity = 0.001f;
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
}

inline void Item::initAnim()
{
	addAnimation(heartItem, "item_smallheart_ani");
}

inline void Item::setItemType(const int type)
{
	itemType = type;
	animationId = itemType;
}

inline void Item::render()
{
	GameObject::render();
	animations[animationId]->render(faceSide, x, y);
}

inline Box Item::getBoundingBox()
{
	return GameObject::getBoundingBox(-1, -1);
}
