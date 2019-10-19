#include "EnemyFish.h"


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
	updateVx();
}

void EnemyFish::updateVx() {
	if (state == walking) vx = faceSide*initVelocity.x;
}

bool EnemyFish::processCollisionWithGround(float minTy, float ny) {
	auto result = Enemy::processCollisionWithGround(minTy, ny);
	if (result) setState(walking);
	return result;
}
