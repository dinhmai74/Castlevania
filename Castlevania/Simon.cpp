#include "Simon.h"
#include "DaggerSubWeapon.h"
#include "StageManager.h"

Simon::Simon()
{
	init();
}

void Simon::init()
{
	whip = new Whip();
	whip->setPosition(x, y);
	isInGround = false;
	isReleaseSitButton = true;
	Simon::initAnim();
	animationId = ANIM_IDLE;
	gravity = SIMON_GRAVITY;
}

void Simon::initAnim()
{
	addAnimation(ANIM_IDLE, "simon_stand_ani");
	addAnimation(ANIM_WALK, "simon_walk_ani");
	addAnimation(ANIM_SIT, "simon_sit_ani");
	addAnimation(ANIM_HITTING, "simon_hitstand_ani");
	addAnimation(ANIM_HITTING_WHEN_SIT, "simon_hitsit_ani");
}

void Simon::render()
{
	if (isHitting) {
		whip->setSide(faceSide);
		whip->render();
	}
	// if (subWeapon)
	// {
	// subWeapon->render();
	// }
	animations[animationId]->render(faceSide, x, y);
	previousAmiId = animationId;
}

void Simon::update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::update(dt);
	whip->update(dt, x, y, nullptr, currentState);
	// if (subWeapon) subWeapon->update(dt, canHitObjects);

	checkCollision(dt, coObjects);
	updateAnimId();
	// simple fall down
	updateGravity(gravity);
}

void Simon::checkCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;
	coEvents.clear();

	calcPotentialCollisions(coObjects, coEvents);

	// no collison
	if (coEvents.empty())
	{
		// case simon is so close the candle when hit
		for (auto& coObject : *coObjects)
		{
			if (coObject->getType() == GameObjectType::item)
			{
				auto coBox = coObject->getBoundingBox();
				if (isColliding(getBoundingBox(), coBox))
				{
					{
						//processCollisionWithItem(dynamic_cast<Item*> (coObject));
					}
				}
			}
		}

		updatePosWhenNotCollide();
	}
	else
	{
		float minTx;
		float minTy;
		float nx = 0;
		float ny;
		filterCollision(coEvents, coEventsResult, minTx, minTy, nx, ny);

		updatePosInTheMomentCollide(minTx, minTy, nx, ny);

		for (auto& i : coEventsResult)
		{
			const auto object = (i->obj);
			const auto boundary = dynamic_cast<Boundary*>(object);
			// process when colis by nx
			if (nx)
			{
				if (boundary) processCollisionWithBoundaryByX(minTx, ny);
			}
			else if (ny)
			{
				if (boundary)
					processCollisionWithGround(minTy, ny);
			}
			/*	if (object->getType() == GameObjectType::item)
				{
					processCollisionWithItem(dynamic_cast<Item*> (object));
				}
				*/
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

	if (ny != 0)
	{
		vy = 0;
		isInGround = true;
		if (currentState == State::jumping)
			standUp();
	}
}

void Simon::processCollisionWithBoundaryByX(float minTx, float ny)
{
}

void Simon::updateAnimId()
{
	if (currentState == walking)
	{
		animationId = ANIM_WALK;
	}
	else if (currentState == sitting)
	{
		if (isReleaseSitButton)
		{
			standUp();
		}
		animationId = ANIM_SIT;
	}
	else if (currentState == jumping)
	{
		// will sit if get enough jump enough high;
		if (vy > 0.15)
			animationId = ANIM_IDLE;
		else
			animationId = ANIM_SIT;
	}
	else if (currentState == hitting)
	{
		// set hitting anim
		animationId = ANIM_HITTING;
		// check and process if animation hitting is done
		if (animations[animationId])
		{
			if (animations[animationId]->isDone())
			{
				whip->refreshAnim();
				animations[animationId]->refresh();
				stand();
				animationId = ANIM_IDLE;
			}
		}
	}
	else if (currentState == hittingWhenSitting)
	{
		animationId = ANIM_HITTING_WHEN_SIT;
		if (animations[animationId])
		{
			const auto frame = animations[animationId]->getFrame();
			if (animations[animationId]->isDone())
			{
				whip->refreshAnim();
				isHitting = false;
				setState(sitting);
				animations[animationId]->refresh();
				animationId = ANIM_SIT;
			}
		}
	}
	else if (currentState == State::throwing)
	{
		animationId = ANIM_HITTING;
		if (animations[animationId])
		{
			const auto frame = animations[animationId]->getFrame();
			if (frame == 2) throwSubWeapon();
			else if (animations[animationId]->isDone())
			{
				animations[animationId]->refresh();
				stand();
			}
		}
	}
	else if (currentState == State::throwingWhenSitting)
	{
		animationId = ANIM_HITTING_WHEN_SIT;
		if (animations[animationId])
		{
			const auto frame = animations[animationId]->getFrame();
			if (frame == 2) throwSubWeapon();
			else if (animations[animationId]->isDone())
			{
				animations[animationId]->refresh();
				setState(State::sitting);
				isThrowing = false;
				animationId = ANIM_IDLE;
			}
		}
	}
	else
	{
		animationId = ANIM_IDLE;
	}
}

void Simon::setState(int state)
{
	GameObject::setState(state);
}

void Simon::move(int side)
{
	faceSide = side;
	setState(walking);
	vx = faceSide * SIMON_VX;
}

void Simon::jump()
{
	setState(jumping);
	vy = -SIMON_VJUMP;
	isInGround = false;
}

void Simon::sit()
{
	setState(sitting);
	vx = 0;
	vy = 0;
}

void Simon::stand()
{
	resetState();
	setState(idle);
	vx = 0;
}

void Simon::standUp()
{
	resetState();
	setState(idle);
}

void Simon::hit()
{
	vx = 0;
	isHitting = true;
	setState(hitting);
}

void Simon::hitWhenSitting()
{
	vx = 0;
	vy = 0;
	isHitting = true;
	setState(hittingWhenSitting);
}

void Simon::throwing()
{
	// if (!subWeapon) return;
	// if (subWeapon->getState() == SubWeaponState::active) return;
		vx = 0;
	 vy = 0;
	 isThrowing = true;
	 setState(State::throwing);
}

void Simon::throwingWhenSitting()
{
	// if (!subWeapon) return;
	// if (subWeapon->getState() == SubWeaponState::active) return;
	 vx = 0;
	 vy = 0;
	 isThrowing = true;
	 setState(State::throwingWhenSitting);
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

void Simon::resetState()
{
	isHitting = false;
	isThrowing = false;
}

Box Simon::getBoundingBox()
{
	return GameObject::getBoundingBox(-1, -1);
}

void Simon::renderBoundingBox()
{
	GameObject::renderBoundingBox();
}

void Simon::upgradeWhipLv(bool up) const
{
	whip->upgradeWhipLv(up);
}


void Simon::handleOnKeyRelease(int KeyCode)
{
	if (KeyCode == DIK_DOWN)
	{
		isReleaseSitButton = true;
		if (isInGround && !isHitting && !isThrowing)
		{
			standUp();
		}
	}
}

void Simon::handleOnKeyPress(BYTE* states)
{
	auto game = Game::getInstance();

	if (isHitting || isThrowing) return;
	if (currentState == sitting) return;
	if (currentState == jumping) return;

	if (game->isKeyDown(DIK_RIGHT))
	{
		move(FaceSide::right);
	}
	else if (game->isKeyDown(DIK_LEFT))
	{
		move(FaceSide::left);
	}
	else if (game->isKeyDown(DIK_DOWN))
	{
		if (isInGround)
		{
			isReleaseSitButton = false;
			sit();
		}
	}

	else
	{
		stand();
	}
}

void Simon::handleOnKeyDown(int keyCode)
{
	if (isHitting || isThrowing) return;

	if (keyCode == DIK_SPACE)
	{
		if (previousState != jumping && isInGround && !isHitting)
		{
			jump();
		}
	}
	else if (keyCode == DIK_LCONTROL)
	{
		if (!isReleaseSitButton)
		{
			hitWhenSitting();
		}
		else
		{
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
			&& currentState != sitting
			&& currentState != jumping
			)
		{
			isReleaseSitButton = false;
			sit();
		}
	}
}

Simon::~Simon()
= default;
