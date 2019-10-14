#include "SubWeaponHolyWater.h"
#include "Boundary.h"

SubWeaponHolyWater::SubWeaponHolyWater() {
	addAnimation(itemHolyWater, "subweapons_holy_water_ani");
	addAnimation(itemHolyWaterEffect, "subweapons_holy_water_effect_ani");
	setWeaponType(itemHolyWater);
	setAnimId(itemHolyWater);
	setState(itemHolyWater);
	timerEffect = new Timer(TIMER_HOLY_BURN_DURATION);
}

void SubWeaponHolyWater::update(DWORD dt, D3DXVECTOR2 simonPos, int simonState, vector<MapGameObjects>& map) {
	GameObject::update(dt);
	updateAnimId();
	checkCollision(dt, map);
	updateGravity(gravity);
	updateEffect();
}

void SubWeaponHolyWater::checkCollision(DWORD dt, vector<MapGameObjects>& maps) {
	for (auto map : maps) {
		switch (map.id) {
		case OBBoundary:
			checkCollisionWithBoundary(dt, map.objs);
			break;
		case OBEnemy:
			checkCollisionWithEnemy(dt, map.objs);
			break;
		default: DebugOut(L"[WARNING] unknown obj to check collision with id %d!\n", map.id);
		}
	}
}

CollisionResult SubWeaponHolyWater::checkCollisionWithBoundary(DWORD dt, vector<GameObject*>* coObjs) {
	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;
	float minTx;
	float minTy;
	float nx = 0;
	float ny;
	coEvents.clear();
	auto result = GameObject::checkCollisionWithBoundary(dt, coObjs, coEventsResult, minTx, minTy, nx, ny);
	bool updatedY = false;
	auto updatedX = false;

	calcPotentialCollisions(coObjs, coEvents);

	// no collison
	if (!coEvents.empty()) {

		filterCollision(coEvents, coEventsResult, minTx, minTy, nx, ny);
		auto isCollideDoor = false;
		// block

			}

	if (!updatedX) {
		if (result.x) blockX(minTx, nx);
		else x += dx;
	}
	if (!updatedY) {
		if (result.y && ny == CDIR_BOTTOM) {
			if (timerEffect->runAlready()) return result;
			setState(itemHolyWaterEffect);
			vx = 0;
			vy = 0;
			gravity = 0.0f;
			timerEffect->start();
		}
		else y += dy;
	}


	for (auto& coEvent : coEvents) delete coEvent;
	return result;
}

void SubWeaponHolyWater::updateEffect() {
	if (timerEffect->isTimeUpAndRunAlr()) {
		disableWeapon();
		isBurned = false;
	}
}

void SubWeaponHolyWater::checkCollisionWithEnemy(DWORD dt, vector<GameObject*>* objs) {
	Weapon::checkCollision(dt, objs);
}

void SubWeaponHolyWater::updateAnimId() {
	setAnimId(state);
}
