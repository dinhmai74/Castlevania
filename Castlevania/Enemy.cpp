#include "Enemy.h"



Enemy::Enemy()
{
	init();
}

Enemy::~Enemy()
{

}

void Enemy::init()
{
	hp = 1;
	setDmg(1);
	setType(enemy);
}


void Enemy::reset()
{
	resetHP();
	resetPos();
}

void Enemy::resetHP()
{
	hp = 1;
}

void Enemy::resetPos()
{
	x = initPos.x;
	x = initPos.y;
}

void Enemy::update(DWORD dt, vector<GameObject*>* coObjects /*= nullptr*/)
{
	GameObject::update(dt);
	if (!isEnable) return;
	checkCollisionAndChangeDirectX(dt,coObjects);
	updateGravity();
}

void Enemy::checkCollisionAndChangeDirectX(DWORD dt, vector<GameObject*>* coObjects)
{
	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;
	coEvents.clear();

	calcPotentialCollisions(coObjects, coEvents);
	// no collison
	if (coEvents.empty())
		updatePosWhenNotCollide();
	else
	{
		float minTx;
		float minTy;
		float nx = 0;
		float ny;
		filterCollision(coEvents, coEventsResult, minTx, minTy, nx, ny);
		updatePosInTheMomentCollide(minTx, minTy, nx, ny);
		changeDirection(coEventsResult, nx, ny);
	}

	for (auto& coEvent : coEvents) delete coEvent;
}

void Enemy::changeDirection(const vector<CollisionEvent*>& vector, float nx, float ny)
{
	if (nx != 0 && ny ==0 )
	{
		faceSide *= -1;
		initSpeed.x = faceSide* initSpeed.x ;
		this->vx = initSpeed.x ;
	}
	else if (ny == -1.0f)
	{
		vy = 0;
	}
}
