#include "EnemyFactory.h"
#include "EnemyGhouls.h"

Enemy* EnemyFactory::getEnemy(int type)
{
	Enemy* enemy;
	auto faceSide = -1;
	auto gravity = E_GRAVITY;
	auto vx = 0.08f;
	auto vy = 0.0f;
	auto dmg = 1;

	switch (type)
	{
	case enemGhouls:
		enemy = new EnemyGhouls();
		dmg = 2;
		break;
	default: enemy = new EnemyGhouls();
	}
	enemy->setGravity(gravity);
	enemy->setFaceSide(faceSide);
	enemy->setInitSpeed({ vx, vy });
	enemy->setSpeed(faceSide * vx, vy);
	enemy->setDmg(dmg);
	enemy->setEnable(false);
	return enemy;
}

EnemyFactory* EnemyFactory::instance = nullptr;

int EnemyFactory::getHp(int enemyType)
{
	switch (enemyType)
	{
	case enemGhouls: return 1;
	default: return 1;
	}
}

