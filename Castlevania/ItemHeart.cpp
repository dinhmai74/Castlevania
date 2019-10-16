#include "ItemHeart.h"



void ItemHeart::update(DWORD dt, vector<GameObject*>* boundary)
{
	GameObject::update(dt);
	checkDurationToDisapear();
	updateGravity(dt);
	checkCollision(dt, boundary);
	if (itemType == itemSmallHeart)checkOutOfBoundary();
}

ItemHeart::ItemHeart(int type)
{
	itemType = type;
	gravity = 0.0002;
	energy = 5;
	setAnimId(type);
}

ItemHeart::ItemHeart()
{
	itemType = itemSmallHeart;
	gravity = 0.0002;
	setAnimId(itemSmallHeart);
	energy = 1;
	faceSide = 1;
}

void ItemHeart::checkOutOfBoundary()
{
	auto distance = fabs(x - initPos.x);
	if (distance >= 15 && !isCollideGround) vx *= -1;
}

int ItemHeart::getEnergy() const
{
	return energy;
}

void ItemHeart::checkCollision(DWORD dt, vector<GameObject*>* boundary)
{
	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;
	coEvents.clear();

	calcPotentialCollisions(boundary, coEvents);
	// no collison
	if (coEvents.empty() && !isCollideGround)
		updatePosWhenNotCollide();
	else
	{
		float minTx;
		float minTy;
		float nx = 0;
		float ny;
		filterCollision(coEvents, coEventsResult, minTx, minTy, nx, ny);
		if(!isCollideGround) updatePosInTheMomentCollide(minTx, minTy, nx, ny);
		if (ny == CDIR_BOTTOM)
			isCollideGround = true;
		else isCollideGround = false;
	}

	for (auto& coEvent : coEvents) delete coEvent;
}

ItemHeart::~ItemHeart() = default;
