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
	updateGravity(dt,gravity);
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

void SubWeaponHolyWater::checkCollisionWithBoundary(DWORD dt, vector<GameObject*>* coObjs) {
	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;
	coEvents.clear();

	calcPotentialCollisions(coObjs, coEvents);

	if (coEvents.empty()) updatePosWhenNotCollide();
	else {
		float minTx;
		float minTy;
		float nx = 0;
		float ny;
		filterCollision(coEvents, coEventsResult, minTx, minTy, nx, ny);
		updatePosInTheMomentCollideAndRemoveVelocity(minTx, minTy, nx, ny);

		for (auto& i : coEventsResult) {
			const auto object = (i->obj);
			processWithBoundary(object, nx, ny);
		}
	}

	for (auto& coEvent : coEvents) delete coEvent;
}

void SubWeaponHolyWater::processWithBoundary(GameObject* const object, float nx, float ny) {
	if (timerEffect->runAlready()) return;
	setState(itemHolyWaterEffect);
	vx = 0;
	vy = 0;
	gravity = 0.0f;
	timerEffect->start();
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
