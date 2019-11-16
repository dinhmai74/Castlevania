#include "SubWeaponBoomerang.h"
#include "Simon.h"

SubWeaponBoomerang::SubWeaponBoomerang()
{
	addAnimation(itemBoomerang, "subweapons_cross_ani");
	setWeaponType(itemBoomerang);
	setAnimId(itemBoomerang);
	territory = { 130,130 };
}

void SubWeaponBoomerang::update(DWORD dt, D3DXVECTOR2 simonPos, int simonState, vector<GameObject*>* coObjects)
{
	GameObject::update(dt);
	checkCollision(dt, coObjects);
	if (!changedDirection)
	{
		const auto distance = fabs(x - initPos.x);
		if (distance >= territory.left)
		{
			vx = -1 * vx;
			faceSide = -1 * getFaceSide();
			changedDirection = true;
		}
	}

	firedDistance += fabs(dx);
}

void SubWeaponBoomerang::checkCollision(DWORD dt, vector<GameObject*>* coObjs)
{
	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;
	coEvents.clear();

	calcPotentialCollisions(coObjs, coEvents);
	calcPotentialCollisionsAABB(coObjs, coEvents);

	if (!coEvents.empty())
	{
		float minTx;
		float minTy;
		float nx = 0;
		float ny;
		filterCollision(coEvents, coEventsResult, minTx, minTy, nx, ny);

		for (auto& i : coEventsResult)
		{
			const auto object = (i->obj);
			processCollisionWithCandle(object, nx, ny);
			processCollisionWithEnemy(object, nx, ny);
			processCollisionWithSimon(object, nx, ny);
		}
	}

	updatePosWhenNotCollide();
	for (auto& coEvent : coEvents) delete coEvent;
}

void SubWeaponBoomerang::processCollisionWithSimon(GameObject* object, float nx, float ny)
{
	auto simon = dynamic_cast<Simon*>(object);

	if (simon && firedDistance >= 100)
	{
		disableWeapon();
	}
}
