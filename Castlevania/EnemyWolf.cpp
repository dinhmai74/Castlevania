#include "EnemyWolf.h"

EnemyWolf::EnemyWolf()
{
	init();
}

EnemyWolf::~EnemyWolf() = default;

void EnemyWolf::initAnim()
{
	addAnimation(idle, "wolf_idle_ani");
	addAnimation(walking, "wolf_run_ani");
}

void EnemyWolf::init()
{
	initAnim();
	setEnemyType(EnemWolf);
	setState(idle);
}

void EnemyWolf::updateAnimId() {

	animId = state;
}

void EnemyWolf::update(DWORD dt, vector<GameObject*>* coObjects) {
	if (state == idle) vx = 0;
	Enemy::update(dt, coObjects);
	updateAnimId();
}
