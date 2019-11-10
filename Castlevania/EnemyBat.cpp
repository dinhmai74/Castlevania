#include "EnemyBat.h"
#include "StageManager.h"


EnemyBat::EnemyBat() {
	setEnemyType(EnemBat);
	initAnim();
	timerAddVelocityY->start();
}

EnemyBat::~EnemyBat()
= default;

void EnemyBat::initAnim() {
	addAnimation(walking, "bat_fly_ani");
	addAnimation(idle, "bat_idle_ani");
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
		vy += initVelocity.y * dt;
}

void EnemyBat::generateEnemy(float playerX, float playerY) {
	auto nx = StageManager::getInstance()->getCurrentStage()->getSimon()->getFaceSide();
	auto posX = playerX + nx * 200;
	auto posY = playerY + 20;
	initPos.y = posY;
	reset();
	setPos(posX, posY);
	setFaceSide(-nx);
	setInitFaceSide(-nx);
	vx = initVelocity.x * faceSide;
	if (!isInViewPort()) return;
	getTimerRespawn()->stop();
	setReadyToRespawn(false);
	setEnable();
}

Box EnemyBat::getBoundingBox() {
	if (isEnable)
		return getBoundingBoxBaseOnFileAndPassWidth(10);

	return { 0,0,0,0 };
}
