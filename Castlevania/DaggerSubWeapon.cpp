#include "DaggerSubWeapon.h"
#include <stdexcept>


DaggerSubWeapon::DaggerSubWeapon()
{
	vx = DAGGER_VX;
	faceSide = FaceSide::right;
	DaggerSubWeapon::initAnim();
}


DaggerSubWeapon::~DaggerSubWeapon()
= default;

void DaggerSubWeapon::checkEnemyCollisions(vector<LPGameObject>* coObjects)
{
	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;
	coEvents.clear();

	calcPotentialCollisions(coObjects, coEvents);

	if (!coEvents.empty()) {
		float minTx, minTy, nx = 0, ny;
		filterCollision(coEvents, coEventsResult, minTx, minTy, nx, ny);
		for (auto& i : coEventsResult)
		{
			const auto item = i->obj;
			item->setState(-1);
		}
	}
	else
	{
		for (auto& coObject : *coObjects)
		{
			auto coBbox = coObject->getBoundingBox();
			auto bbox=getBoundingBox();
			if (isColliding(coBbox,bbox)) {
				coObject->setState(-1);
			}
		}
	}

	for (auto& coEvent : coEvents) delete coEvent;

}

void DaggerSubWeapon::render()
{
	float camX, camY;
	Game::getInstance()->getCameraPosition(camX, camY);
	if (x<camX - DAGGER_W || x>camX + SCREEN_WIDTH) setState(SubWeaponState::inActive);
	if (currentState == SubWeaponState::inActive) return;
	animationId = ANIM_DAGGER_L;
	animations[animationId]->render(faceSide,x, y);
}

RECT DaggerSubWeapon::getBoundingBox()
{
	return GameObject::getBoundingBox(DAGGER_W, DAGGER_H);
}

void DaggerSubWeapon::update(const DWORD dt, vector<GameObject*>* coObjects)
{
	GameObject::update(dt);
	if (currentState == inActive) return;
	vx = faceSide == FaceSide::left ? -DAGGER_VX : DAGGER_VX;
	x += dx;
	checkEnemyCollisions(coObjects);
}

void DaggerSubWeapon::initAnim()
{
	addAnimation(ANIM_DAGGER_L);
	addAnimation(ANIM_DAGGER_R);
}
