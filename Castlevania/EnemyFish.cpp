#include "EnemyFish.h"
#include "Bullet.h"
#include "StageManager.h"


EnemyFish::EnemyFish() {
	initAnim();
	setEnemyType(EnemFish);
	jumpingMaxRange = 180;
}

EnemyFish::~EnemyFish() = default;

void EnemyFish::initAnim() {
	addAnimation(walking, "fish_walk_ani");
	addAnimation(hitting, "fish_hit_ani");
	addAnimation(jumping, "fish_jump_ani");
}

void EnemyFish::updateAnimId() {
	auto tempAnimId = state == idle ? jumping : state;
	setAnimId(tempAnimId);
}

void EnemyFish::updateWhenJumping() {
	if (state == jumping) {
		if (initPos.y - y >= jumpingMaxRange) {
			setState(idle);
			vy = 0;
			return;
		}
		vy = initVelocity.y;
		vx = 0;
	}
}

void EnemyFish::update(DWORD dt, vector<GameObject*> * coObjects) {
	updateRespawnPosBaseOnSim();
	updateAnimId();
	Enemy::update(dt, coObjects);
	updateWhenJumping();
	updateWalking();
	shoot();
}

void EnemyFish::updateRespawnPosBaseOnSim() {
	auto sim = StageManager::getInstance()->getCurrentStage()->getSimon();
	auto nx = sim->getFaceSide();
	auto playerX = sim->getPos().x;
	auto posX = playerX - nx * activeRange;
	initPos.x = posX;
	if (!isInViewPort() && IsEnable()) {
		setPos(posX, initPos.y);
		setFaceSide(nx);
		setInitFaceSide(nx);
		setState(jumping);
	}
}

void EnemyFish::updateWalking() {
	if (state == walking) vx = faceSide * initVelocity.x;
	else if (state == hitting) {
		vx = 0;
		if (animations[animId]->isDone()) {
			setState(walking);
			faceSide *= -1;
			animations[hitting]->refresh();
		}
	}
}

bool EnemyFish::processCollisionWithGround(float minTy, float ny) {
	auto result = Enemy::processCollisionWithGround(minTy, ny);
	if (result && state != hitting) {
		setState(walking);
	}
	return result;
}

void EnemyFish::generateEnemy(float playerX, float playerY) {
	auto nx = StageManager::getInstance()->getCurrentStage()->getSimon()->getFaceSide();
	auto posX = playerX - nx * activeRange;
	auto posY = y;
	reset();
	setPos(posX, posY);
	initPos.x = posX;
	setFaceSide(nx);
	setInitFaceSide(nx);
	vx = initVelocity.x * faceSide;
	if (!isInViewPort()) return;
	getTimerRespawn()->stop();
	setReadyToRespawn(false);
	setEnable();
}

void EnemyFish::shoot() {
	if (canShoot()) {
		setState(hitting);
		generateBullet();
		auto time = rand() % 2500 + 2000;
		timerShooting->setLimitedTime(time);
		timerShooting->start();
	}
	else {
		if (!timerShooting->isRunning()) timerShooting->start();
	}
}

bool EnemyFish::canShoot() {
	return ShootingEnemy::canShoot() && state == walking;
}
