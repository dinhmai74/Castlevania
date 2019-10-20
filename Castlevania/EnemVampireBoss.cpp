#include "EnemVampireBoss.h"
#include "StageManager.h"


EnemyVampireBoss::EnemyVampireBoss() {
	init();
}

EnemyVampireBoss::~EnemyVampireBoss() = default;

void EnemyVampireBoss::init() {
	initAnim();
	setType(OBBoss);
	setState(sleep);
	setAnimId(sleep);
	setEnable();
	setFaceSide(SideLeft);
	setHp(15);
	setInitGravity(0);
	setGravity(0);
	setInitSpeed({ 1.7f, 1.7f });
	setScore(300);
	setDmg(3);
}

void EnemyVampireBoss::initAnim() {
	addAnimation(sleep, "vampire_boss_sleep_ani");
	addAnimation(flying, "vampire_boss_fly_ani");
	addAnimation(hitting, "vampire_boss_fly_ani");
}

void EnemyVampireBoss::update(DWORD dt, vector<GameObject*> * coObjects /*= nullptr*/) {
	checkCanAwake();
	checkAttack();
	updateVelocityWhenAttack();
	GameObject::update(dt);
}


void EnemyVampireBoss::checkCanAwake() {
	if (state != sleep) return;
	auto stage = StageManager::getInstance()->getCurrentStage();
	simon = stage->getSimon();

	if (simon->getPosition().x - x >= 50) {
		setState(flying);
		setAnimId(flying);
	}
}

void EnemyVampireBoss::checkAttack() {
}

void EnemyVampireBoss::updateVelocityWhenAttack() {
	if(state==hitting)	{
		auto pos = simon->getPosition();
		auto nx = pos.x - x;
		auto ny = pos.y - y;

		vx = nx * getInitSpeed().x;
		vy = ny * getInitSpeed().y;
	}
}
