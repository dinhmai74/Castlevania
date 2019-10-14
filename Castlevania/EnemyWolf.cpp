#include "EnemyWolf.h"
#include "ForceEnemyJumpingBound.h"

EnemyWolf::EnemyWolf() {
	init();
}

EnemyWolf::~EnemyWolf() = default;

void EnemyWolf::initAnim() {
	addAnimation(idle, "wolf_idle_ani");
	addAnimation(walking, "wolf_run_ani");
}

void EnemyWolf::init() {
	initAnim();
	setEnemyType(EnemWolf);
	setState(idle);
}

void EnemyWolf::updateAnimId() {
	setAnimId(state);
}

void EnemyWolf::update(DWORD dt, vector<GameObject*> * coObjects, float simX) {
	if (state == idle) vx = 0;
	Enemy::update(dt, coObjects);

	checkCollisonWithBoundaryForceJump(coObjects);
	updateStateWhenSimGoTerritory(simX);
	updateAnimId();
}

void EnemyWolf::updateStateWhenSimGoTerritory(float simX) {
	auto rightSide = simX - x > 0;
	auto distance = fabs(simX - x);
	auto activeDistance = rightSide ? activeTerritory.right : activeTerritory.left;
	auto canActive = distance <= activeDistance;
	if (canActive) {
		setState(walking);
		vx = initVelocity.x * faceSide;
	}
}

void EnemyWolf::checkCollisonWithBoundaryForceJump(vector<GameObject*> * coObjects) {
	for (auto obj : *coObjects) {
		auto force = dynamic_cast<ForceEnemyJumpingBound*>(obj);
		if (!force) continue;
		if (isColliding(obj->getBoundingBox(), getBoundingBox())) {
			if (isInGround) jump();
			break;
		}
	}
}

void EnemyWolf::jump() {
	vy = initVelocity.y;
	isInGround = false;
	setState(jumping);
}
