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
	setEnemyType(enemGhouls);
	initAnim();
	setState(walking);
	animId = ANIM_WALK;
}
