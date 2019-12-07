#include "Enemy.h"
#include <stdexcept>
#include "Boundary.h"
#include "EnemyFactory.h"
#include "SimonConstants.h"
#include "StageManager.h"

Enemy::Enemy() { Enemy::init(); }

Enemy::~Enemy() = default;

void Enemy::init() {
	setActive();
	setEnable(false);
	resetHp();
	setDmg(2);
	setType(OBEnemy);
	forceRespawn = false;
	setRespawnTime(E_RESPAWN_INIT_TIME);
	timerUntouchable = new Timer(E_UNTOUCHABLE_DURATION);
	setTimerRespawn(new Timer(respawnTime));
	getTimerRespawn()->start();
	initAnim();
}

void Enemy::reset() {
	resetHp();
	resetPos();
	setState(initState);
	setAnimId(getInitAnimId());
	setFaceSide(initFaceSide);
	vx = initVelocity.x * initFaceSide;
	readyToRespawn = false;
}

void Enemy::resetHp() { hp = EnemyFactory::getInstance()->getHp(enemyType); }

void Enemy::resetPos() {
	x = initPos.x;
	y = initPos.y;
}

void Enemy::update(DWORD dt, vector<GameObject*> * coObjects /*= nullptr*/) {
	if (isStopAllAction) return;
	GameObject::update(dt, coObjects);
	if (!isEnable) return;
	processBaseOnState();
	checkCollisionAndChangeDirectX(dt, coObjects);
	checkCollisionWithWater(coObjects);
	updateGravity(dt);
}

void Enemy::checkCollisionAndChangeDirectX(DWORD dt,
	vector<GameObject*> * coObjects) {
	vector<LPCollisionEvent> coEventsResult;
	vector<LPCollisionEvent> coEvents;
	coEvents.clear();
	CollisionResult result = { false, false };
	bool updatedY = false;
	auto updatedX = false;
	float minTx;
	float minTy;
	float nx = 0;
	float ny;

	calcPotentialCollisions(coObjects, coEvents);

	if (!coEvents.empty()) {
		filterCollision(coEvents, coEventsResult, minTx, minTy, nx, ny);

		for (auto& i : coEventsResult) {
			const auto boundary = dynamic_cast<Boundary*>(i->obj);
			if (boundary) {
				auto type = boundary->getBoundaryType();
				switch (type) {
				case BoundaryNormal:
					result.x = processCollisionWithBoundaryByX(minTx, nx,
						boundary);
					break;
				case BoundaryGround:
					result.y = processCollisionWithGround(minTy, ny);
					break;
				default:;
				}
			}
		}
	}
	if (!updatedX) {
		if (result.x)
			changeDirection(minTx, nx);
		else
			x += dx;
	}
	if (!updatedY) {
		if (result.y && ny == CDIR_BOTTOM) {
			isInGround = true;
			vy = 0;
		}
		else
			y += dy;
	}
}

void Enemy::changeDirection(float nx, float ny) {
	faceSide = getFaceSide() * -1;
	this->vx *= -1;
}

void Enemy::respawn(float playerX, float playerY) {
	if (canRespawn({ playerX, playerY })) generateEnemy(playerX, playerY);
}

void Enemy::generateEnemy(float playerX, float playerY) {
	auto nx = playerX - initPos.x > 0 ? 1 : -1;

	reset();
	setFaceSide(nx);
	setInitFaceSide(nx);
	vx = initVelocity.x * nx;
	if (!isInViewPort()) return;
	setEnable();
	readyToRespawn = false;
}

bool Enemy::canRespawn(D3DXVECTOR2 simPos) {
	const auto isEnoughTime = getTimerRespawn()->isTimeUpAndRunAlr();
	if (isEnoughTime && !isEnable) readyToRespawn = true;
	const auto distance = fabs(initPos.x - simPos.x);
	const auto isInRegion =
		distance >= respawnArea.min && distance <= respawnArea.max;

	return ((readyToRespawn && isInRegion) || forceRespawn ) ;
}

void Enemy::processWhenBurnedEffectDone() {
	if (burnEffect && burnEffect->isOver(BURNED_DURATION)) {
		burnEffect = nullptr;
		if (state != death) {
			reset();
			setEnable(false);
		}
	}
}

void Enemy::setEnable(bool val /*= true*/) {
	if (!val && isEnable) {
		timerRespawn->start();
	}
	GameObject::setEnable(val);
}

bool Enemy::isInViewPort() {
	const auto camPosition = Game::getInstance()->getCameraPosition();
	const auto offset = 20;
	const auto left = camPosition.x + offset;
	const auto right = camPosition.x + SCREEN_WIDTH - offset;

	auto box = getBoundingBoxBaseOnFile();
	const auto isInView = isColliding(
		box, { left, camPosition.y, right, camPosition.y + SCREEN_HEIGHT });

	return isInView;
}

void Enemy::doDeathAnim() {
	StageManager::getInstance()->addScore(score);
	GameObject::doDeathAnim();
}

void Enemy::initAnim() {
	addAnimation(ANIM_EMPTY, "empty_ani");
}

void Enemy::updateAnimId() {
	if (state == death) return;
	setAnimId(state);
}

Box Enemy::getBoundingBox() {
	if (!isEnable) return { 0, 0, 0, 0 };
	auto tempBox = getBoundingBoxBaseOnFileAndPassWidth(20);

	tempBox.t += 10;
	return tempBox;
}

void Enemy::processBaseOnState() {
	if (state == death) {
		reset();
		setEnable(false);
	}
}
