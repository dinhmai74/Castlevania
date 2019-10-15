#include "Enemy.h"
#include <stdexcept>
#include "EnemyFactory.h"
#include "SimonConstants.h"


Enemy::Enemy() {
	Enemy::init();
}

Enemy::~Enemy() = default;

void Enemy::init() {
	setActive();
	setEnable(false);
	resetHp();
	setDmg(1);
	setType(OBEnemy);
	forceRespawn = false;
	setRespawnTime(E_RESPAWN_INIT_TIME);
	timerUntouchable = new Timer(E_UNTOUCHABLE_DURATION);
	setTimerRespawn(new Timer(respawnTime));
	getTimerRespawn()->start();
}

void Enemy::reset() {
	resetHp();
	resetPos();
	setState(initState);
	setAnimId(ANIM_WALK);
	timerRespawn->setLimitedTime(respawnTime);
	if (!timerRespawn->isRunning()) timerRespawn->start();
	setFaceSide(initFaceSide);
	vx = initVelocity.x * initFaceSide;
}

void Enemy::resetHp() {
	hp = EnemyFactory::getInstance()->getHp(enemyType);
}

void Enemy::resetPos() {
	x = initPos.x;
	y = initPos.y;
}

void Enemy::update(DWORD dt, vector<GameObject*> * coObjects /*= nullptr*/) {
	if (getIsStopAllAction()) return;
	GameObject::update(dt);
	if (!isEnable) return;
	if (vx > 0) setIsVirgin(false);
	checkCollisionAndChangeDirectX(dt, coObjects);
	updateGravity(this->initGravity);
}

void Enemy::checkCollisionAndChangeDirectX(DWORD dt, vector<GameObject*> * coObjects) {
	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;
	float minTx;
	float minTy;
	float nx = 0;
	float ny;
	coEvents.clear();

	auto result = GameObject::checkCollisionWithBoundary(dt, coObjects, coEventsResult, minTx, minTy, nx, ny);
	bool updatedY = false;
	auto updatedX = false;

	if (!updatedX) {
		if (result.x) changeDirection(coEventsResult,minTx, nx);
		else x += vx*dt;
	}
	if (!updatedY) {
		if (result.y && ny == CDIR_BOTTOM) {
			blockY(minTy, ny);
		}
		else y += dy;
	}
}

void Enemy::changeDirection(const vector<CollisionEvent*> & vector, float nx, float ny) {
	if (nx != 0) {
		faceSide = getFaceSide() * -1;
		this->vx *= -1;
	}
	else if (ny == -1.0f) {
		vy = 0;
	}
}

void Enemy::respawn(float playerX, float playerY) {
	if (canRespawn({ playerX, playerY })) {
		generateEnemy(playerX);
	}
}

void Enemy::generateEnemy(float playerX) {
	auto nx = playerX - x > 0 ? 1 : -1;
	reset();
	setFaceSide(nx);
	setInitFaceSide(nx);
	vx = initVelocity.x * nx;
	getTimerRespawn()->stop();
	setEnable();
}

bool Enemy::canRespawn(D3DXVECTOR2 simPos) {
	const auto isEnoughTime = getTimerRespawn()->isTimeUpAndRunAlr();
	const auto distance = fabs(x - simPos.x);
	const auto isInRegion = distance >= respawnArea.min && distance <= respawnArea.max;

	return (isEnoughTime && isInRegion && !isEnable) || forceRespawn;
}

void Enemy::processWhenBurnedEffectDone() {
	if (burnEffect && burnEffect->isOver(BURNED_DURATION)) {
		burnEffect = nullptr;
		setEnable(false);
		reset();
	}
}

void Enemy::setEnable(bool val /*= true*/) {
	GameObject::setEnable(val);
	if (!val)
		getTimerRespawn()->start();
}

void Enemy::render() {
	if (isEnable) {
		alpha = 255;
		if (isStopAllAction && currentFrame)
			animations[animId]->render(getFaceSide(), x, y, currentFrame, alpha, r, g, b);
		else animations[animId]->render(getFaceSide(), x, y, alpha);

		currentFrame = animations[animId]->getCurrentFrame();
	}
	if (burnEffect) {
		const auto blowX = x;
		const auto blowY = y;
		burnEffect->render(1, blowX, blowY);
	}
}
