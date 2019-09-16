#include "Simon.h"
#include "DaggerSubWeapon.h"
#include "StageManager.h"

Simon::Simon()
{
	whip = new Whip();
	whip->setPosition(x, y);
	isInGround = false;
	isReleaseSitButton = true;
	Simon::initAnim();
	animationId = ANIM_SIM_IDLE_R;
}

Simon::~Simon()
= default;

void Simon::setState(int state)
{
	GameObject::setState(state);
}

void Simon::handleOnKeyRelease(int KeyCode)
{
	if (KeyCode == DIK_DOWN) {
		isReleaseSitButton = true;
		if (isInGround && !isHitting && !isThrowing) {
			standUp();
		}
	}
}

void Simon::handleOnKeyPress(BYTE * states)
{
	auto game = Game::getInstance();

	if (isHitting || isThrowing) return;
	if (currentState == SimonState::sitting) return;
	if (currentState == SimonState::jumping) return;

	if (game->isKeyDown(DIK_RIGHT)) {
		move(FaceSide::right);
	}
	else if (game->isKeyDown(DIK_LEFT)) {
		move(FaceSide::left);
	}
	else if (game->isKeyDown(DIK_DOWN)) {
		if (isInGround) {
			isReleaseSitButton = false;
			sit();
		}
	}

	else {
		stand();
	}
}

void Simon::handleOnKeyDown(int keyCode)
{
	if (isHitting || isThrowing) return;

	if (keyCode == DIK_SPACE) {
		if (previousState != SimonState::jumping&& isInGround && !isHitting) {
			jump();
		}
	}
	else if (keyCode == DIK_LCONTROL) {
		if (!isReleaseSitButton) {
			hitWhenSitting();
		}
		else {
			hit();
		}
	}
	else if (keyCode == DIK_A)
	{
		if (!isReleaseSitButton)
			throwingWhenSitting();
		else throwing();
	}
	else if (keyCode == DIK_DOWN)
	{
		if (isInGround
			&& currentState != SimonState::sitting
			&& currentState != SimonState::jumping
			)
		{
			isReleaseSitButton = false;
			sit();
		}
	}
}

void Simon::move(int side)
{
	faceSide = side;
	setState(SimonState::walking);
	vx = faceSide == FaceSide::left ? -SIMON_VX : SIMON_VX;
}

void Simon::jump()
{
	setState(SimonState::jumping);
	vy = -SIMON_VJUMP;
	isInGround = false;
}

void Simon::sit()
{
	y = y + (SIM_MOVE_H - SIM_SIT_H);
	setState(SimonState::sitting);
	vx = 0;
	vy = 0;
}

void Simon::stand()
{
	resetState();
	setState(SimonState::idle);
	vx = 0;
}

void Simon::standUp()
{
	resetState();
	setState(SimonState::idle);
	y = y - (SIM_MOVE_H - SIM_SIT_H);
}

void Simon::hit()
{
	vx = 0;
	isHitting = true;
	setState(SimonState::hitting);
}

void Simon::hitWhenSitting()
{
	vx = 0;
	vy = 0;
	isHitting = true;
	setState(SimonState::hittingWhenSitting);
}

void Simon::throwing()
{
	// if (!subWeapon) return;
	// if (subWeapon->getState() == SubWeaponState::active) return;
//	vx = 0;
	// vy = 0;
	// isThrowing = true;
	// setState(SimonState::throwing);
}

void Simon::throwingWhenSitting()
{
	// if (!subWeapon) return;
	// if (subWeapon->getState() == SubWeaponState::active) return;
	// vx = 0;
	// vy = 0;
	// isThrowing = true;
	// setState(SimonState::throwingWhenSitting);
}

void Simon::throwSubWeapon()
{
/*	if (!subWeapon) return;
	if (subWeapon->getState() == SubWeaponState::active) return;

	subWeapon->setSide(faceSide);
	const auto subX = faceSide == FaceSide::left ? x - SIM_HIT_W + 10 : x + SIM_HIT_W - 10;
	const auto subY = y + 16;

	subWeapon->setPosition(subX, subY);
subWeapon->setState(SubWeaponState::active);
	*/
}

void Simon::beginFight()
{
}

void Simon::resetState()
{
	isHitting = false;
	isThrowing = false;
}

void Simon::update(DWORD dt, vector<LPGAMEOBJECT>* coObjects,
	vector<LPGAMEOBJECT>* canHitObjects)
{
	GameObject::update(dt);
	return;
	whip->update(dt, x, y, canHitObjects);
	// if (subWeapon) subWeapon->update(dt, canHitObjects);

	checkCollision(dt, coObjects);
	auto newPositionX = x + dx;
	if (newPositionX >= 0 && newPositionX + SIM_MOVE_W <= boundingGameX) {
		x = newPositionX;
	}
	updateAnimId();
	// simple fall down
	if(!isInGround) vy += dt * SIMON_GRAVITY;
}


void Simon::checkCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;
	coEvents.clear();

	calcPotentialCollisions(coObjects, coEvents);

	// no collison
	if (coEvents.empty()) {
		// case simon is so close the candle when hit
		for (auto& coObject : *coObjects)
		{
			if (coObject->getType() == GameObjectType::item)
			{
				auto coBox=coObject->getBoundingBox();
				if (isColliding(getBoundingBox(),coBox)) {
					{
						//processCollisionWithItem(dynamic_cast<Item*> (coObject));
					}
				}
			}
		}
		y += dy;
	}
	else {
		float minTx;
		float minTy;
		float nx = 0;
		float ny;
		filterCollision(coEvents, coEventsResult, minTx, minTy, nx, ny);

		for (auto& i : coEventsResult)
		{
			const auto object = (i->obj);
		/*	if (object->getType() == GameObjectType::item)
			{
				processCollisionWithItem(dynamic_cast<Item*> (object));
			}
			else */if (object->getType() == GameObjectType::brick)
			{
				processCollisionWithGround(minTy, ny);
			}
			else
			{
				y += minTy * dy + ny * 0.4f;
				x += minTx * dx + nx * 0.4;
			}
		}
	}

	for (auto& coEvent : coEvents) delete coEvent;
}

//void Simon::processCollisionWithItem(Item* item)
//{
//	if (item->getItemType() == ItemType::heartItem) {
//	}
//	else if (item->getItemType() == ItemType::whipItem) {
//		if (whip)whip->upgradeWhipLv();
//	}
//	else if (item->getItemType() == ItemType::daggerItem) {
//		// subWeapon = new DaggerSubWeapon();
//	}
//	item->setState(State::dead);
//}

void Simon::processCollisionWithGround(float minTy, float ny)
{
	y += minTy * dy + ny * 0.4f;

	if (ny != 0) {
		vy = 0;
		isInGround = true;
		if (currentState == SimonState::jumping)
			standUp();
	}
}


void Simon::render()
{
	return;
	if (isHitting) {
		whip->setSide(faceSide);
		whip->render();
	}
	// if (subWeapon)
	// {
		// subWeapon->render();
	// }
	animations[animationId]->render(faceSide,x, y);
	previousAmiId = animationId;
}

void Simon::updateAnimId()
{

	if (currentState == SimonState::walking) {
		animationId = faceSide == FaceSide::left
			? ANIM_SIM_WALKING_L
			: ANIM_SIM_WALKING_L;
	}
	else if (currentState == SimonState::sitting) {
		if (isReleaseSitButton) {
			standUp();
		}
		animationId = faceSide == FaceSide::left
			? ANIM_SIM_SITTING_L
			: ANIM_SIM_SITTING_L;
	}
	else if (currentState == SimonState::jumping) {
		if (vy > 0.15)
		{
			animationId = faceSide == FaceSide::left
				? ANIM_SIM_IDLE_L
				: ANIM_SIM_IDLE_L;
		}
		else
		{
			animationId = faceSide == FaceSide::left
				? ANIM_SIM_SITTING_L
				: ANIM_SIM_SITTING_L;
		}
	}
	else if (currentState == SimonState::hitting) {
		// set hitting anim
		animationId = faceSide == FaceSide::left
			? ANIM_SIM_HITTING_L
			: ANIM_SIM_HITTING_L;
		// check and process if animation hitting is done
		if (animations[animationId]) {
			if (animations[animationId]->isDone()) {
				whip->refreshAnim();
				animations[animationId]->refresh();
				stand();
				animationId = faceSide == FaceSide::left
					? ANIM_SIM_IDLE_L
					: ANIM_SIM_IDLE_L;
			}
		}
	}
	else if (currentState == SimonState::hittingWhenSitting) {
		animationId = faceSide == FaceSide::left
			? ANIM_SIM_HIT_WHEN_SITTING_L
			: ANIM_SIM_HIT_WHEN_SITTING_L;
		if (animations[animationId]) {
			const auto frame = animations[animationId]->getFrame();
			if (animations[animationId]->isDone()) {
				whip->refreshAnim();
				isHitting = false;
				setState(SimonState::sitting);
				animations[animationId]->refresh();
				animationId = faceSide == FaceSide::left
					? ANIM_SIM_IDLE_L
					: ANIM_SIM_IDLE_L;
			}
		}
	}
	else if (currentState == SimonState::throwing)
	{
		animationId = faceSide == FaceSide::left
			? ANIM_SIM_HITTING_L
			: ANIM_SIM_HITTING_R;
		if (animations[animationId]) {
			const auto frame = animations[animationId]->getFrame();
			if (frame == 2) throwSubWeapon();
			else if (animations[animationId]->isDone())
			{
				animations[animationId]->refresh();
				stand();
			}
		}
	}
	else if (currentState == SimonState::throwingWhenSitting)
	{
		animationId = faceSide == FaceSide::left
			? ANIM_SIM_HIT_WHEN_SITTING_L
			: ANIM_SIM_HIT_WHEN_SITTING_R;
		if (animations[animationId]) {
			const auto frame = animations[animationId]->getFrame();
			if (frame == 2) throwSubWeapon();
			else if (animations[animationId]->isDone())
			{
				animations[animationId]->refresh();
				setState(SimonState::sitting);
				isThrowing = false;
				animationId = faceSide == FaceSide::left
					? ANIM_SIM_IDLE_L
					: ANIM_SIM_IDLE_R;
			}
		}
	}
	else {
		animationId = faceSide == FaceSide::left
			? ANIM_SIM_IDLE_L
			: ANIM_SIM_IDLE_R;
	}
}
RECT Simon::getBoundingBox()
{
	// do more get bbox here
	float width, height;

	if (currentState == SimonState::sitting ||
		(currentState == SimonState::jumping) ||
		currentState == SimonState::hittingWhenSitting ||
		currentState == SimonState::throwingWhenSitting
		) {
		width = SIM_SIT_W;
		height = SIM_SIT_H;
	}
	else {
		width = SIM_MOVE_W;
		height = SIM_MOVE_H;
	}

	return {static_cast<LONG>(x), static_cast<LONG>(y), static_cast<LONG>(x + width), static_cast<LONG>(y + height)};
}

void Simon::initAnim()
{
}
