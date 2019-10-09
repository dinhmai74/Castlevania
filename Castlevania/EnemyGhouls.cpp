#include "EnemyGhouls.h"


void EnemyGhouls::initAnim()
{
	addAnimation(ANIM_WALK, "shouls_walk_ani");
}

EnemyGhouls::EnemyGhouls()
{
	init();
}

EnemyGhouls::~EnemyGhouls() = default;

void EnemyGhouls::init()
{
	setEnemyType(EnemGhouls);
	initAnim();
	setState(walking);
	setAnimId(ANIM_WALK);
}
