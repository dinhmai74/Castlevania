#include "EnemyBat.h"
#include "StageManager.h"


EnemyBat::EnemyBat() {
	setEnemyType(EnemBat);
	initAnim();
	setState(walking);
	setAnimId(walking);
	timerAddVelocityY->start();
}

EnemyBat::~EnemyBat()
= default;

void EnemyBat::initAnim() {
	addAnimation(walking, "bat_fly_ani");
}

void EnemyBat::updateAnimId() {
}

void EnemyBat::update(DWORD dt, vector<GameObject*> * coObjects) {
	updateAnimId();
	GameObject::update(dt);
	x += vx;
	y += dy;
	updateVy();
	updateGravity(dt);
}

void EnemyBat::updateVy() {
	const auto distance = initPos.y - y;
	const auto offset = 2;
	isTop = distance >= offset;

	if (isTop) {
		isBottom = false;
		return;
	}

	if (isBottom == false) isBottom = -distance >= offset;

	if (isBottom)
		vy += initVelocity.y*dt;
}

void EnemyBat::generateEnemy(float playerX, float playerY) {
	auto nx = -StageManager::getInstance()->getCurrentStage()->getSimon()->getFaceSide();
	reset();
	setFaceSide(nx);
	setInitFaceSide(nx);
	auto posX = playerX - nx* 200;
	auto posY = playerY + 20;
	setPosition(posX,posY);
	initPos.y = posY;
	vx = initVelocity.x * nx;
	getTimerRespawn()->stop();
	setEnable();
}
