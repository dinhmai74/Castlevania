#include "Weapon.h"

void Weapon::disableWeapon()
{
	setActive(false);
	setEnable(false);
}

void Weapon::processWithCandle(GameObject* coObject, int nx, int ny)
{
	auto candle = dynamic_cast<Candle*>(coObject);
	if (candle) candle->getHurt(nx, ny, getDmg());
}

void Weapon::checkCollision(DWORD dt, vector<GameObject*>* coObjs)
{
	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;
	coEvents.clear();

	calcPotentialCollisions(coObjs, coEvents);
	calcPotentialCollisionsAABB(coObjs, coEvents);

	if (coEvents.empty()) updatePosWhenNotCollide();
	else
	{
		float minTx;
		float minTy;
		float nx = 0;
		float ny;
		filterCollision(coEvents, coEventsResult, minTx, minTy, nx, ny);
		updatePosWhenNotCollide();

		for (auto& i : coEventsResult)
		{
			const auto object = (i->obj);
			processWithCandle(object, nx, ny);
		}

		if (remainHit != -1)
		{
			remainHit--;
			if (remainHit <= 0) disableWeapon();
		}
	}

	for (auto& coEvent : coEvents) delete coEvent;
}

void Weapon::update(DWORD dt, D3DXVECTOR2 simonPos, int simonState, vector<GameObject*>* coObjects)
{
	GameObject::update(dt);
	checkCollision(dt, coObjects);
}