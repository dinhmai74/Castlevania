#include "EnemyBat.h"
#include "StageManager.h"

auto stageManager= StageManager::getInstance();

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
	int nx;
	auto pos = getPosBaseOnSimon( nx);
	initPos.y = pos.y;
	reset();
	setPos(pos.x, pos.y);
	setFaceSide(-nx);
	setInitFaceSide(-nx);
	vx = initVelocity.x * faceSide;
	if (!isInViewPort()) return;
	getTimerRespawn()->stop();
	setReadyToRespawn(false);
	setEnable();
}

D3DXVECTOR2 EnemyBat::getPosBaseOnSimon(int& nx) {
	auto simon = stageManager->getCurrentStage()->getSimon();
	nx = simon->getFaceSide();
	auto pos = simon->getPos();
	auto posX = pos.x+ nx * 200;
	auto posY = pos.y+10;
	return { posX,posY };
}

Box EnemyBat::getBoundingBox() {
	if (isEnable)
		return getBoundingBoxBaseOnFileAndPassWidth(2);

	return { 0,0,0,0 };
}
