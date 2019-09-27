#include "EnemyGhouls.h"


void EnemyGhouls::initAnim()
{
	addAnimation(walking, "shouls_walk_ani");
}

EnemyGhouls::EnemyGhouls()
{
	init();
}

EnemyGhouls::~EnemyGhouls() = default;

void EnemyGhouls::init()
{
	setEnemyType(enemGhouls);
	initAnim();
	animId = walking;
}
