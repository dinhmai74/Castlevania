#include "EnemyFactory.h"
#include "EnemyGhouls.h"
#include "EnemyWolf.h"
#include "EnemyBat.h"

Enemy* EnemyFactory::getEnemy(int type) {
	Enemy* enemy;
	auto faceSide = -1;
	auto gravity = E_GRAVITY;
	auto vx = 0.08f;
	auto vy = 0.0f;
	auto dmg = 1;
	auto score = 200;

	switch (type) {
	case EnemGhouls:
		enemy = new EnemyGhouls();
		dmg = 2;
		break;
	case EnemWolf:
		enemy = new EnemyWolf();
		dmg = 3;
		gravity = 0.0009;
		vx = 0.27f;
		vy = -0.22f;
		break;
	case EnemBat:
		enemy = new EnemyBat();
		dmg = 2;
		gravity = 0.0003;
		vx = 1.7f;
		vy = -0.0005f;
		break;
	default: enemy = new EnemyGhouls();
	}
	enemy->setRespawnTime(100);
	enemy->setGravity(gravity);
	enemy->setFaceSide(faceSide);
	enemy->setInitSpeed({ vx, vy });
	enemy->setSpeed(faceSide * vx, 0);
	enemy->setDmg(dmg);
	enemy->setEnable(false);
	enemy->setInitGravity(gravity);
	enemy->setScore(score);
	return enemy;
}

EnemyFactory* EnemyFactory::instance = nullptr;

int EnemyFactory::getHp(int enemyType) {
	switch (enemyType) {
	default: return 1;
	}
}
