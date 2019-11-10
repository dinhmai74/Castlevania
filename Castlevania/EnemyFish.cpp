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
	updateAnimId();
	Enemy::update(dt, coObjects);
	updateWhenJumping();
	updateWalking();
	shoot();
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

void EnemyFish::generateBullet() {
	auto bullet = new Bullet();
	bullet->setFaceSide(faceSide);
	auto box = getBoundingBox();
	auto width = box.r - box.l;
	const auto xBullet = faceSide> 0 ? x +width + 5 : x;
	bullet->setPos(xBullet,y+10);

	StageManager::getInstance()->add(bullet);
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
	return timerShooting->isTimeUpAndRunAlr() && state == walking;
}
