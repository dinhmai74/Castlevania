#include "EnemyFactory.h"
#include "EnemyGhouls.h"
#include "EnemyWolf.h"

Enemy* EnemyFactory::getEnemy(int type) {
	Enemy* enemy;
	auto faceSide = -1;
	auto gravity = E_GRAVITY;
	auto vx = 0.08f;
	auto vy = 0.0f;
	auto dmg = 1;

	switch (type) {
	case EnemGhouls:
		enemy = new EnemyGhouls();
		dmg = 2;
		enemy->getTimerRespawn()->setLimitedTime(1000);
		break;
	case EnemWolf:
		enemy = new EnemyWolf();
		dmg = 3;
		gravity = 0.0009;
		vx = 0.27f;
		vy = -0.22f;
		enemy->getTimerRespawn()->setLimitedTime(100);
		break;
	default: enemy = new EnemyGhouls();
	}
	enemy->setGravity(gravity);
	enemy->setFaceSide(faceSide);
	enemy->setInitSpeed({ vx, vy });
	enemy->setSpeed(faceSide * vx, 0);
	enemy->setDmg(dmg);
	enemy->setEnable(false);
	enemy->setInitGravity(gravity);
	return enemy;
}

EnemyFactory* EnemyFactory::instance = nullptr;

int EnemyFactory::getHp(int enemyType) {
	switch (enemyType) {
	default: return 1;
	}
}
