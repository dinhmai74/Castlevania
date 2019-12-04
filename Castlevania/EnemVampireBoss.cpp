#include "EnemVampireBoss.h"
#include "StageManager.h"
#include "ObjectEndGame.h"


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
	untouchableDuration = 200;
	setHp(15);
	setInitGravity(0);
	setGravity(0);
	setInitSpeed({ 0.1f, 0.1f });
	setScore(300);
	setDmg(2);
	nextTargetPos = { -1,-1 };
}

void EnemyVampireBoss::initAnim() {
	addAnimation(sleep, "vampire_boss_sleep_ani");
	addAnimation(flying, "vampire_boss_fly_ani");
	addAnimation(hitting, "vampire_boss_fly_ani");
	addAnimation(idle, "vampire_boss_fly_ani");
	addAnimation(death, "vampire_boss_death_ani");
}

void EnemyVampireBoss::update(DWORD dt, vector<GameObject*> * coObjects /*= nullptr*/) {
	checkCanAwake();
	getNewActionBaseOnState();
	getNextPositionToFly();
	updateVelocity();
	GameObject::update(dt);
	x += dx;
	y += dy;
	setAnimId(state);
	processDeathEffect();
	alpha = 255;
}


void EnemyVampireBoss::checkCanAwake() {
	if (state != sleep) return;
	auto stage = StageManager::getInstance()->getCurrentStage();
	simon = stage->getSimon();

	if (simon->getPos().x - x >= 50) {
		setIdle();
		auto cam = Camera::getInstance();
		cam->lock();
		auto pos = cam->getPos();
		cam->setLimitX({ pos.x,pos.x + SCREEN_WIDTH });
		stage->setIsFightingBoss();
	}
}

void EnemyVampireBoss::getNextPositionToFly() {
	if (nextTargetPos.x > 0 && nextTargetPos.y > 0) return;

	if (state == hitting) {
		nextTargetPos = simon->getPos();
	}
	else if (state == flying) {
		nextTargetPos = getRandomPosInBound();
	}
	else nextTargetPos = { -1,-1 };
}

void EnemyVampireBoss::updateVelocity() {
	if (state == idle || state == sleep || state == death) {
		vx = vy = 0;
		return;
	}
	auto pos = nextTargetPos;
	auto nx = x - pos.x > 0 ? -1 : 1;
	auto ny = y - pos.y > 0 ? -1 : 1;

	if (state == flying) {
		vx = nx * getInitSpeed().x;
		vy = ny * getInitSpeed().y;
	}
	else if (state == hitting) {

		vx = nx * getInitSpeed().x * 1.5;
		vy = ny * getInitSpeed().y * 1.5;
	}
}

D3DXVECTOR2 EnemyVampireBoss::getRandomPosInBound() {
	D3DXVECTOR2 randomSpot;

	float left = initPos.x - RANDOMSPOT_AREA;
	float top = initPos.y;

	float distance = 0;

	do {
		randomSpot.x = left + rand() % (2 * RANDOMSPOT_AREA);
		randomSpot.y = top + rand() % (RANDOMSPOT_AREA);

		float dx = abs(x - randomSpot.x);
		float dy = abs(y - randomSpot.y);

		distance = sqrt(pow(dx, 2) + pow(dy, 2));
	} while (distance < 100.0f);

	return randomSpot;
}

void EnemyVampireBoss::setIdle() {
	setState(idle);
	if (!timerIdle->isRunning())timerIdle->start();
}

Box EnemyVampireBoss::getBoundingBox() {
	if (!IsEnable() || state ==death) return { x,y,x,y };
	return getBoundingBoxBaseOnFileAndPassWidth(50);
}

void EnemyVampireBoss::getNewActionBaseOnState() {
	if (state == death) return;
	if (timerIdle->isTimeUpAndRunAlr()) {
		timerIdle->stop();
		setState(hitting);
	}

	auto offset = 10;
	auto gotToNextPos = fabs(x - nextTargetPos.x) <= offset && fabs(y - nextTargetPos.y) <= offset;

	if (state == hitting) {
		if (gotToNextPos)  setState(flying);
	}
	else if (state == flying) {
		if (gotToNextPos)  setIdle();
	}
}

void EnemyVampireBoss::setState(int state) {
	GameObject::setState(state);
	nextTargetPos = { -1,-1 };
}

void EnemyVampireBoss::processDeathEffect() {
	if (isDying()) {
		vx = 0;
		vy = 0;
		startDying = true;
	}
	else if (startDying) {
		startDying = false;
		auto obj = new ObjectEndGame();
		auto xPos = initPos.x + (getWidth() / 2);
		auto yPos = initPos.y+ 80;
		obj->setPos(xPos,yPos);
		StageManager::getInstance()->add(obj);
		setDisable();
	}
}

