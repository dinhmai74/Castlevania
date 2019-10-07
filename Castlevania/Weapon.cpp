#include "Weapon.h"
#include "Enemy.h"

void Weapon::disableWeapon()
{
	setActive(false);
	setEnable(false);
}

bool Weapon::processCollisionWithCandle(GameObject* coObject, int nx, int ny)
{
	auto candle = dynamic_cast<Candle*>(coObject);
	if (candle) {
		candle->getHurt(nx, ny, getDmg());
		return true;
	}

	return false;
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
		auto hit = false;

		for (auto& i : coEventsResult)
		{
			const auto object = (i->obj);
			hit = processCollisionWithEnemy(object,nx,ny);
			if(!hit) hit=processCollisionWithCandle(object, nx, ny);
		}

		if (remainHit != -1 && hit)
		{
			remainHit--;
			if (remainHit <= 0 && hit) disableWeapon();
		}
	}

	for (auto& coEvent : coEvents) delete coEvent;
}

void Weapon::update(DWORD dt, D3DXVECTOR2 simonPos, int simonState, vector<GameObject*>* coObjects)
{
	GameObject::update(dt);
	checkCollision(dt, coObjects);
}

bool Weapon::processCollisionWithEnemy(GameObject* object, float nx, float ny)
{
	auto enemy = dynamic_cast<Enemy*>(object);
	if (enemy) {
		enemy->getHurt(nx, ny, getDmg());
		return true;
	}

	return false;
}
