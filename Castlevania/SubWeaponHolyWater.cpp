#include "SubWeaponHolyWater.h"
#include "Boundary.h"

SubWeaponHolyWater::SubWeaponHolyWater()
{
	addAnimation(itemHolyWater, "subweapons_holy_water_ani");
	addAnimation(itemHolyWaterEffect, "subweapons_holy_water_effect_ani");
	setWeaponType(itemHolyWater);
	setAnimId(itemHolyWater);
	setState(itemHolyWater);
	timerEffect = new Timer(TIMER_HOLY_BURN_DURATION);
}

void SubWeaponHolyWater::update(DWORD dt, D3DXVECTOR2 simonPos, int simonState, vector<GameObject*>* coObjects)
{
	GameObject::update(dt);
	updateAnimId();
	checkCollision(dt, coObjects);
	updateGravity(gravity);
	updateEffect();
}

void SubWeaponHolyWater::checkCollision(DWORD dt, vector<GameObject*>* coObjs)
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

		for (auto& i : coEventsResult)
		{
			const auto object = (i->obj);
			auto boundary = dynamic_cast<Boundary*>(object);
			if (boundary)
			{
				processWithBoundary(object, nx, ny);
			}
			else
			{
				processCollisionWithCandle(object, nx, ny);
				updatePosWhenNotCollide();
			}
		}
	}

	for (auto& coEvent : coEvents) delete coEvent;
}

void SubWeaponHolyWater::checkCollisionWithBoundary(DWORD dt, vector<GameObject*>* coObjs)
{
	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;
	coEvents.clear();

	calcPotentialCollisions(coObjs, coEvents);

	if (coEvents.empty()) updatePosWhenNotCollide();
	else
	{
		float minTx;
		float minTy;
		float nx = 0;
		float ny;
		filterCollision(coEvents, coEventsResult, minTx, minTy, nx, ny);
		updatePosInTheMomentCollideAndRemoveVelocity(minTx, minTy, nx, ny);

		for (auto& i : coEventsResult)
		{
			const auto object = (i->obj);
			processWithBoundary(object, nx, ny);
		}
	}

	for (auto& coEvent : coEvents) delete coEvent;
}

void SubWeaponHolyWater::updateEffect()
{
	if (timerEffect->isTimeUpAndRunAlr())
	{
		disableWeapon();
		isBurned = false;
	}
}

void SubWeaponHolyWater::updateAnimId()
{
	setAnimId(state);
}

void SubWeaponHolyWater::processWithBoundary(GameObject* const object, float nx, float ny)
{
	if (timerEffect->runAlready()) return;
	setState(itemHolyWaterEffect);
	vx = 0;
	vy = 0;
	gravity = 0.0f;
	timerEffect->start();
}