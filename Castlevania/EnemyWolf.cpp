#include "EnemyWolf.h"
#include "ForceEnemyJumpingBound.h"

EnemyWolf::EnemyWolf() {
	init();
}

EnemyWolf::~EnemyWolf() = default;

void EnemyWolf::initAnim() {
	addAnimation(idle, "wolf_idle_ani");
	addAnimation(walking, "wolf_run_ani");
	addAnimation(jumping, "wolf_jump_ani");
}

void EnemyWolf::init() {
	initAnim();
	setEnemyType(EnemWolf);
	setState(idle);
}

void EnemyWolf::updateAnimId() {
	setAnimId(state);
}

void EnemyWolf::update(DWORD dt, vector<GameObject*>* coObjects, float simX) {
	if (state == idle) {
		vx = 0;
		gravity = initGravity;
	}
	Enemy::update(dt, coObjects);

	checkIfCanRun(simX);
	checkCollisionWithGround(dt, coObjects);
	checkCollisonWithBoundaryForceJump(coObjects);
	updateVelocity();
	updateAnimId();
}

void EnemyWolf::checkIfCanRun(float simX) {
	auto rightSide = simX - x > 0;
	auto distance = fabs(simX - x);
	auto activeDistance = rightSide ? activeTerritory.right : activeTerritory.left;
	auto canActive = distance <= activeDistance;
	if (canActive) {
		canRun = true;
	}
}

void EnemyWolf::checkCollisonWithBoundaryForceJump(vector<GameObject*>* coObjects) {
	for (auto obj : *coObjects) {
		auto force = dynamic_cast<ForceEnemyJumpingBound*>(obj);
		if (!force) continue;
		if (isColliding(obj->getBoundingBox(), getBoundingBox())) {
			if (isInGround && state == walking) jump();
			break;
		}
	}
}

void EnemyWolf::jump() {
	vy = initVelocity.y;
	isInGround = false;
	jumped = true;
	setState(jumping);
}

void EnemyWolf::checkCollisionWithGround(DWORD dt, vector<GameObject*>* coObjects) {
	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventResult;

	calcPotentialCollisions(coObjects, coEvents);
	if (!coEvents.empty()) {
		float minTx, minTy, nx, ny;
		filterCollision(coEvents, coEventResult, minTx, minTy, nx, ny);
		for (auto rs : coEventResult) {
			auto obj = dynamic_cast<Boundary*>(rs->obj);
			if (obj && obj->getBoundaryType() == BoundaryGround && ny == CDIR_BOTTOM) {
				if (canRun) {
					setState(walking);
					if (jumped && !changedFaceSide) {
						faceSide *= -1;
						changedFaceSide = true;
					}
				}
				else setState(idle);
			}
		}
	}
}

void EnemyWolf::updateVelocity() {
	if (!canRun) return;
	if (state != jumping) setState(walking);
	vx = initVelocity.x * faceSide;
}

void EnemyWolf::reset() {
	Enemy::reset();
	jumped = false;
	changedFaceSide = false;
	setState(idle);
}
