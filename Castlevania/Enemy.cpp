#include "Enemy.h"
#include <stdexcept>
#include "EnemyFactory.h"
#include "SimonConstants.h"
#include "Boundary.h"
#include "StageManager.h"


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
	updateGravity(dt);
}

void Enemy::checkCollisionAndChangeDirectX(DWORD dt, vector<GameObject*> * coObjects) {
	vector<LPCollisionEvent> coEventsResult;
	vector<LPCollisionEvent> coEvents;
	coEvents.clear();
	CollisionResult result = { false,false };
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
					result.x = processCollisionWithBoundaryByX(minTx, nx, boundary);
					break;
				case BoundaryGround:
					result.y =processCollisionWithGround(minTy, ny);
					break;
				default:;
				}
			}
		}
	}
	if (!updatedX) {
		if (result.x) changeDirection(minTx, nx);
		else x += dx;
	}
	if (!updatedY) {
		if (result.y && ny == CDIR_BOTTOM) {
			isInGround = true;
			vy = 0;
		}
		else y += dy;
	}
}

void Enemy::changeDirection(float nx, float ny) {
	if (nx != 0) {
		faceSide = getFaceSide() * -1;
		this->vx *= -1;
	}
}

void Enemy::respawn(float playerX, float playerY) {
	if(enemyType==EnemBat)
	{
		DebugOut(L"bat \n");
	}
	if (canRespawn({ playerX, playerY })) {
		generateEnemy(playerX,playerY);
	}
}

void Enemy::generateEnemy(float playerX,float playerY) {
	if (!isInViewPort()) return;
	auto nx = playerX - initPos.x> 0 ? 1 : -1;
	reset();
	setFaceSide(nx);
	setInitFaceSide(nx);
	vx = initVelocity.x * nx;
	getTimerRespawn()->stop();
	setEnable();
	readyToRespawn = false;
}

bool Enemy::canRespawn(D3DXVECTOR2 simPos) {
	const auto isEnoughTime = getTimerRespawn()->isTimeUpAndRunAlr();
	if (isEnoughTime) readyToRespawn = true;
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

bool Enemy::isInViewPort() {
	const auto camPosition = Game::getInstance()->getCameraPosition();
	const auto left = camPosition.x;
	const auto right = camPosition.x + SCREEN_WIDTH;

	auto box = getBoundingBoxBaseOnFile();
	const auto isInView = isColliding(box,
		{
			left, camPosition.y, right,
			camPosition.y + SCREEN_HEIGHT
		});


	return isInView;
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
