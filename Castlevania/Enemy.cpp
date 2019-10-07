#include "Enemy.h"
#include <stdexcept>
#include "EnemyFactory.h"


Enemy::Enemy()
{
	init();
}

Enemy::~Enemy()
{

}

void Enemy::init()
{
	resetHP();
	setDmg(1);
	setType(enemy);
	setRespawnTime(3000);
	timerRespawn = new Timer(respawnTime);
	timerRespawn->start();
}

void Enemy::reset()
{
	resetHP();
	resetPos();
	setState(walking);
	setAnimId(ANIM_WALK);
	setFaceSide(initFaceSide);
	vx = initSpeed.x * initFaceSide;
}

void Enemy::resetHP()
{
	hp = EnemyFactory::getHp(enemyType);
}

void Enemy::resetPos()
{
	x = initPos.x;
	y = initPos.y;
}

void Enemy::update(DWORD dt, vector<GameObject*>* coObjects /*= nullptr*/)
{
	if (getIsStopAllAction()) return;
	GameObject::update(dt);
	if (!isEnable) return;
	checkCollisionAndChangeDirectX(dt, coObjects);
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
		x += minTx * dx + nx * 0.1f;
		y += minTy * dy + ny * 0.1f;
		changeDirection(coEventsResult, nx, ny);
	}

	for (auto& coEvent : coEvents) delete coEvent;
}

void Enemy::changeDirection(const vector<CollisionEvent*>& vector, float nx, float ny)
{
	if (nx != 0)
	{
		faceSide = getFaceSide() * -1;
		this->vx *= -1;
	}
	else if (ny == -1.0f)
	{
		vy = 0;
	}
}


void Enemy::respawn(float playerX, float playerY)
{
	if (canRespawn({ playerX,playerY }))
	{
		reset();
		setNewEnemy();
	}
}

bool Enemy::canRespawn(D3DXVECTOR2 simPos)
{
	const auto isEnoughTime = timerRespawn->isTimeUpAndRunAlr();
	const auto distance = fabs(x - simPos.x);
	const auto isInRegion = distance >= respawnArea.min && distance <= respawnArea.max;

	return isEnoughTime && isInRegion && !isEnable;
}

void Enemy::setNewEnemy(bool val /*= true*/)
{
	timerRespawn->stop();
	setEnable();
}

void Enemy::getHurt(int nx, int ny, int hpLose)
{
	GameObject::getHurt(nx, ny, hpLose);
}

void Enemy::processWhenBurnedEffectDone()
{
	if (burnEffect && burnEffect->isOver(BURNED_DURATION))
	{
		burnEffect = nullptr;
		setEnable(false);
		reset();
	}
}

void Enemy::setEnable(bool val /*= true*/)
{
	GameObject::setEnable(val);
	if (!val) {
		timerRespawn->start();
	}
}

void Enemy::render()
{
	if (burnEffect)
	{
		const auto blowX = x;
		const auto blowY = y;
		burnEffect->render(1, blowX, blowY);
	}
	if (!isEnable) return;
	if (isStopAllAction && currentFrame)
	{
		animations[animId]->render(getFaceSide(), x, y, currentFrame, alpha, r, g, b);
	}
	else animations[animId]->render(getFaceSide(), x, y, alpha);

	currentFrame = animations[animId]->getCurrentFrame();

}
