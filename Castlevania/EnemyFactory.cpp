#include "EnemyFactory.h"
#include "EnemyGhouls.h"
#include "EnemyWolf.h"

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
	case EnemGhouls:
		enemy = new EnemyGhouls();
		dmg = 2;
		break;
	case EnemWolf:
		enemy = new EnemyWolf();
		dmg = 3;
		vx = 0.15f;
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
	default: return 1;
	}
}

