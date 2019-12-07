#pragma once
#include "GameObject.h"
#include <algorithm>
#include "SimonConstants.h"
#include "StageManager.h"
#include "Bubbles.h"

void GameObject::processWhenBurnedEffectDone() {
	if (timerBurnEffect->isTimeUpAndRunAlr()) {
		burnEffect = nullptr;
		if (state == death) setActive(false);
		timerBurnEffect->stop();
	}
}

void GameObject::doUntouchable() {
	if (untouchableDuration > 0) timerUntouchable->start(untouchableDuration);
	else timerUntouchable->start();
}

void GameObject::doUntouchable(DWORD time) {
	timerUntouchable->start(time);
}

GameObject::GameObject() {
	x = y = 0;
	initPos = { 0, 0 };
	vx = vy = 0;
	alpha = r = b = g = 255;
	setFaceSide(SideRight); // right side
	isEnable = true;
	preAnimId = -1;
	previousAnimIsOneTimeAnim = false;
	boundingGameX = 0;
	boundingGameY = 0;
	state = normal;
	isActive = true;
	gravity = 0;
	setHp(1);
	untouchableDuration = 2000;
	deflectTimeDuration = 400;
	deathTimeDuration = 1000;
	timerUntouchable = new Timer(untouchableDuration);
	timerDeflect = new Timer(deflectTimeDuration);
	timerDeath = new Timer(deathTimeDuration);
	timerBurnEffect = new Timer(BURNED_DURATION);
	vxDeflect = 0.15f;
	vyDeflect = 0.06f;
	nxDeflect = -1;
	burnAnimId = ANIM_BURNED;
	addAnimation(ANIM_EMPTY, "empty_ani");
}

GameObject::~GameObject() {
	if (texture != nullptr) texture->Release();
}

void GameObject::updateAnimId() {
	if (state == deflect && animations[ANIM_DEFLECT]) setAnimId(ANIM_DEFLECT);
	else if (state == death && animations[ANIM_DEATH]) setAnimId(ANIM_DEATH);
}

void GameObject::render() {
	if (IsEnable()) {
		animations[animId]->render(faceSide, x, y, alpha, 255, 255, 255, isStopAllAction, currentFrame);
		currentFrame = animations[animId]->getCurrentFrame();
	}
	if (burnEffect) {
		auto getCenterPos = getCenter();
		const auto blowX = getCenterPos.x;
		const auto blowY = getCenterPos.y;
		burnEffect->render(1, blowX, blowY);
	}
}

void GameObject::getSpeed(float& vx, float& vy) const {
	vx = this->vx;
	vy = this->vy;
}

void GameObject::getPos(float& x, float& y) const {
	x = this->x;
	y = this->y;
}

void GameObject::doBurnedEffect(bool enable) {
	timerBurnEffect->start();
	const auto now = GetTickCount64();
	burnEffect = AnimationManager::getInstance()->get(burnAnimId);
	burnEffect->setAniStartTime(now);
	setEnable(enable);
}

bool GameObject::processCollisionWithGround(float minTy, float ny) {
	if (ny == CDIR_BOTTOM) {
		isInGround = true;
		gravity = initGravity;
		return true;
	}

	return false;
}

bool GameObject::processCollisionWithBoundaryByX(float minTx, float nx, GameObject* boundary) {
	if (nx == 0) return false;
	return true;
}

void GameObject::setDeathByWater() {
	setState(death);
	animId = ANIM_EMPTY;
	vx = 0;
	vy = 0;
	hp = 0;
}

D3DXVECTOR2 GameObject::getCenter() {
	auto box = getBoundingBoxBaseOnFile();
	auto xPos = x + ((box.r - box.l) / 2);
	auto yPos = y;
	return { xPos, yPos };
}

void GameObject::renderBoundingBox() {
	const auto texture = TextureManager::getInstance()->get(ID_TEX_BBOX);
	const auto rect = getBoundingBox();

	Game::getInstance()->draw(getFaceSide(), rect.l, rect.t, texture, rect, rect, 140);
}

void GameObject::addAnimation(int id, string animTexId) {
	animations[id] = AnimationManager::getInstance()->get(animTexId);
}

bool GameObject::getHurt(int nx, int hpLose) {
	return getHurt(nx, 1, hpLose);
}

bool GameObject::getHurt(int nx, int ny, int hpLose) {
	if (isUntouching() || isDeflecting() || hpLose <= 0) return false;
	loseHp(hpLose);
	if (this->getHp() <= hpLose)
		doDeathAnim();
	else setStatusWhenStillHaveEnoughHP(nx, hpLose);
	return true;
}

void GameObject::doDeathAnim() {
	if (state == death) return;
	setState(death);
	vx = 0;
	vy = 0;
	this->setHp(0);
	if (animations[ANIM_DEATH]) timerDeath->start();
	else doBurnedEffect();
}

void GameObject::loseHp(int hpLose) {
	setHp(getHp() - hpLose);
	if (getHp() <= 0) setHp(0);
}

void GameObject::setStatusWhenStillHaveEnoughHP(int nx, int hpLose) {
	if (animations[ANIM_DEFLECT] && canDeflect) {
		doDeflect(nx);
		doBurnedEffect(true);
	}
	else {
		doUntouchable();
		doBurnedEffect(true);
	}
}

void GameObject::doDeflect(int nx) {
	setState(deflect);
	if (nx == 0) nx = 1;
	gravity = initGravity;
	setNxDeflect(nx);
	timerDeflect->start();
}

LPCollisionEvent GameObject::sweptAABBEx(LPGAMEOBJECT coO) {
	float t, nx, ny;

	auto coBox = coO->getBoundingBox();

	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	coO->getSpeed(svx, svy);

	float sdx = svx * dt;
	float sdy = svy * dt;

	float dx = this->dx - sdx;
	float dy = this->dy - sdy;

	auto obBox = getBoundingBox();

	sweptAABB(
		obBox,
		coBox,
		dx, dy,
		t, nx, ny
	);

	auto* e = new CollisionEvent(t, nx, ny, coO);
	return e;
}

/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void GameObject::calcPotentialCollisions
(vector<LPGAMEOBJECT>* coObjects, vector<LPCollisionEvent>& coEvents) {
	for (UINT i = 0; i < coObjects->size(); i++) {
		auto e = sweptAABBEx(coObjects->at(i));

		if (e->t >= 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CollisionEvent::compare);
}

void GameObject::calcPotentialCollisionsAABB(vector<LPGAMEOBJECT>* coObjects, vector<LPCollisionEvent>& coEvents) {
	for (UINT i = 0; i < coObjects->size(); i++) {
		auto ob = coObjects->at(i);
		auto isCollided = isColliding(getBoundingBox(), ob->getBoundingBox());
		if (isCollided) {
			auto* e = new CollisionEvent(0, 1, 1, ob);
			coEvents.push_back(e);
		}
	}
}

void GameObject::filterCollision
(vector<LPCollisionEvent>& coEvents,
	vector<LPCollisionEvent>& coEventsResult,
	float& min_tx, float& min_ty, float& nx, float& ny) {
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++) {
		auto c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			// (thời gian va chạm), (nx != 0 có va chạm theo trục x)
			min_tx = c->t;
			nx = c->nx;
			min_ix = i;
		}

		if (c->t < min_ty && c->ny != 0) {
			min_ty = c->t;
			ny = c->ny;
			min_iy = i;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}

void GameObject::checkCollisionWithWater(vector<LPGAMEOBJECT>* coObjects) {
	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;
	coEvents.clear();

	calcPotentialCollisions(coObjects, coEvents);
	if (!coEvents.empty()) {
		float minTx;
		float minTy;
		float nx = 0;
		float ny;
		filterCollision(coEvents, coEventsResult, minTx, minTy, nx, ny);

		for (auto& i : coEventsResult) {
			auto obj = i->obj;
			if (obj->getType() != OBWater) continue;
			auto posX = x + getWidth();
			auto posY = obj->y;
			StageManager::getInstance()->add(new Bubbles(posX, posY));
			if (ny == CDIR_BOTTOM) setDeathByWater();
		}
	}

	for (auto& coEvent : coEvents) delete coEvent;
}

void GameObject::update(const DWORD dt, vector<GameObject*>* coObject) {
	this->dt = dt;
	createBlowUpEffectAndSetRespawnTimer();
	processUntouchableEffect();
	processWhenBurnedEffectDone();
	dx = vx * dt;
	dy = vy * dt;
}

void GameObject::processDeathEffect() {
	if (isDying()) {
		startDying = true;
		vx = 0;
		vy = 0;
	}
}

void GameObject::processUntouchableEffect() {
	if (isUntouching()) alpha = rand() % 255;
	else alpha = 255;
}

void GameObject::processDeflectEffect() {
	if (timerDeflect->isTimeUpAndRunAlr()) {
		doActionAfterDeflect();
	}
	else if (timerDeflect->isRunning()) {
		vx = vxDeflect * nxDeflect;
		vy = -vyDeflect;
		faceSide = -nxDeflect;
	}
}

void GameObject::createBlowUpEffectAndSetRespawnTimer() {
	if (timerBurnEffect->isTimeUpAndRunAlr()) {
		timerBurnEffect->stop();
		burnEffect = nullptr;
	}
}

void GameObject::checkCollisionAndStopMovement(DWORD dt, vector<GameObject*>* coObjects) {
	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;
	coEvents.clear();

	calcPotentialCollisions(coObjects, coEvents);
	// no collison
	if (coEvents.empty())
		updatePosWhenNotCollide();
	else {
		float minTx;
		float minTy;
		float nx = 0;
		float ny;
		filterCollision(coEvents, coEventsResult, minTx, minTy, nx, ny);
		updatePosInTheMomentCollideAndRemoveVelocity(minTx, minTy, nx, ny);
	}

	for (auto& coEvent : coEvents) delete coEvent;
}

void GameObject::updatePosWhenNotCollide() {
	x += dx;
	y += dy;
}

void GameObject::updateGravity(DWORD dt, float gravity) {
	vy += gravity * dt;
	this->gravity = gravity;
}

void GameObject::updatePosInTheMomentCollideAndRemoveVelocity(float minTx, float minTy, float nx, float ny) {
	updatePosInTheMomentCollide(minTx, minTy, nx, ny);
	if (nx != 0) vx = 0;
	if (ny != 0) vy = 0;
}

void GameObject::updatePosInTheMomentCollide(float minTx, float minTy, float nx, float ny) {
	blockX(minTx, nx);
	blockY(minTy, ny);
}

Box GameObject::getBoundingBoxBaseOnFile() {
	if (!animations[animId]) return { 0, 0, 0, 0 };
	float r, l;
	auto spriteFrame = animations[animId]->getFrameSprite();
	auto spriteBoundary = animations[animId]->getFrameBoundingBox();
	auto offset = getOffsetFromBoundingBox();

	if (getFaceSide() == SideRight) {
		r = x + (spriteFrame.r - spriteFrame.l) - offset.x;
		l = r - (spriteBoundary.r - spriteBoundary.l);
	}
	else {
		l = x + offset.x;
		r = spriteBoundary.r - spriteBoundary.l + l;
	}

	const float t = y + offset.y;
	const float b = t + (spriteBoundary.b - spriteBoundary.t);

	return Box(l, t, r, b);
}

Box GameObject::getBoundingBoxBaseOnFileAndPassWidth(float width) {
	const auto box = getBoundingBoxBaseOnFile();
	const float bboxWidth = box.r - box.l;
	const float l = x + bboxWidth / 2 - width / 2;
	const float r = l + width;
	return Box(l, box.t, r, box.b);
}

D3DXVECTOR2 GameObject::getOffsetFromBoundingBox() {
	const auto spriteFrame = animations[animId]->getFrameSprite();
	const auto spriteBoundary = animations[animId]->getFrameBoundingBox();

	// spriteFrame is usually larger than the spriteBoundary so we need to take account of the offset
	const auto offsetX = spriteBoundary.l - spriteFrame.l;
	const auto offsetY = spriteBoundary.t - spriteFrame.t;
	return { offsetX, offsetY };
}
