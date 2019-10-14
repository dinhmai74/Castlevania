#include "EnemyWolf.h"

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

	animId = state;
}

void EnemyWolf::update(DWORD dt, vector<GameObject*> * coObjects, float simX) {
	if (state == idle) vx = 0;
	Enemy::update(dt, coObjects);

	updateStateWhenSimGoTerritory(simX);
	updateAnimId();
}

void EnemyWolf::updateStateWhenSimGoTerritory(float simX) {
	auto rightSide = simX - x > 0;
	auto distance = fabs(simX - x);
	auto activeDistance= rightSide ? activeTerritory.right: activeTerritory.left;
	DebugOut(L"activeDistance %f\n", activeDistance);
	DebugOut(L"distance %f\n", distance);
	auto canActive = distance <= activeDistance;
	if (canActive) {
		setState(walking);
		vx = initVelocity.x * faceSide;
	}
}
