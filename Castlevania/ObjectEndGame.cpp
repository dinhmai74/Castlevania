#include "ObjectEndGame.h"


ObjectEndGame::ObjectEndGame() {
	initAnim();
	setAnimId(idle);
	state = idle;
	setType(OBEndGame);
	vx = 0;
	gravity= 0.0005f;
}
ObjectEndGame::~ObjectEndGame() {}
void ObjectEndGame::initAnim() {
	addAnimation(idle, "objects_endgame_idle_ani");
}

void ObjectEndGame::update(DWORD dt, vector<GameObject*>* coObjects) {
	GameObject::update(dt);

	checkCollisionAndStopMovement(dt,coObjects);

	updateGravity(dt);
}
