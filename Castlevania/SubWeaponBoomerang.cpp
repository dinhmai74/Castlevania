#include "SubWeaponBoomerang.h"

SubWeaponBoomerang::SubWeaponBoomerang()
{
	setWeaponType(itemBoomerang);
	animId = itemBoomerang;
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
			faceSide = -1 * faceSide;
			changedDirection = true;
		}
	}
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
			processWithCandle(object, nx, ny);
		}
	}

	updatePosWhenNotCollide();
	for (auto& coEvent : coEvents) delete coEvent;
}