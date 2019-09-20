#include "Simon.h"
#include "StageManager.h"
#include "SubWeaponDagger.h"

auto subWeaponFactory = SubWeaponFactory::getInstance();

Simon::Simon()
{
	init();
}

void Simon::init()
{
	whip = new Whip();
	subWeapon = subWeaponFactory->getSubWeapon(itemDagger);
	getSubWeapon()->setEnable(false);
	whip->setPosition(x, y);

	isInGround = false;
	isReleaseSitButton = true;
	Simon::initAnim();
	animId = ANIM_IDLE;
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
	if (isHitting && collectingWhipTimer ->IsTimeUp())
	{
		whip->setSide(faceSide);
		whip->render();
	}

	DebugOut(L"\n %d", forceRenderFrame);
	animations[animId]->render(faceSide, x, y, alpha, r, g, b);
	currentFrame = animations[animId]->getCurrentFrame();

	preAnimId = animId;
}

void Simon::update(DWORD dt, const vector<MapGameObjects>& maps)

{
	updateRGB();
	if (!collectingWhipTimer->IsTimeUp()) return;
	GameObject::update(dt);

	checkCollision(dt, maps);
	updateAnimId();
	// simple fall down
	updateGravity(gravity);
}

void Simon::updateRGB()
{
	if (!collectingWhipTimer->IsTimeUp())
	{
		r = g + 80;
		g = 100 + rand() % 150;
		b = g + 35;
		animations[animId]->setRenderFrame(forceRenderFrame);
	}
	else
	{
		r = g = b = 255;
		isCollectingWhip = false;
		collectingWhipTimer->Stop();
	}
}

void Simon::checkCollision(DWORD dt, const vector<MapGameObjects>& maps)
{
	for (auto map : maps)
	{
		switch (map.id)
		{
		case boundary: checkCollisionWithBoundary(dt, map.objs);
			break;
		case item: checkCollisionWithItems(dt, map.objs);
			break;
		case canHitObjs: updateWeaponAction(dt, map.objs);
			break;
		default: DebugOut(L"[WARNING] unknown obj to check collision with id %d!\n", map.id);
		}
	}
}

void Simon::checkCollisionWithBoundary(DWORD dt, vector<LPGAMEOBJECT>* boundaries)
{
	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;
	coEvents.clear();

	calcPotentialCollisions(boundaries, coEvents);

	// no collison
	if (coEvents.empty()  )
	{
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
			if (!boundary) continue;
			if (nx)
				processCollisionWithBoundaryByX(minTx, ny);
			else if (ny)
				processCollisionWithGround(minTy, ny);
		}
	}

	for (auto& coEvent : coEvents) delete coEvent;
}

void Simon::checkCollisionWithItems(DWORD dt, vector<GameObject*>* items)
{
	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;
	coEvents.clear();

	calcPotentialCollisions(items, coEvents);

	// no collison => check case inside
	if (coEvents.empty())
		for (auto& coObject : *items)
		{
			const auto item = dynamic_cast<Item*>(coObject);
			if (item)
			{
				auto coBox = item->getBoundingBox();
				if (isColliding(getBoundingBox(), coBox))
					processCollisionWithItem(item);
			}
		}
	else
	{
		float minTx;
		float minTy;
		float nx = 0;
		float ny;
		filterCollision(coEvents, coEventsResult, minTx, minTy, nx, ny);

		for (auto& i : coEventsResult)
		{
			const auto object = (i->obj);
			const auto item = dynamic_cast<Item*>(object);
			if (item)
			{
				auto coBox = item->getBoundingBox();
				if (isColliding(getBoundingBox(), coBox))
					processCollisionWithItem(item);
			}
		}
	}

	for (auto& coEvent : coEvents) delete coEvent;
}

void Simon::updateWeaponAction(DWORD dt, vector<GameObject*>* objs)
{
	whip->update(dt, x, y, objs, currentState);
}

void Simon::processCollisionWithItem(Item* item)
{
	const auto itemType = item->getItemType();
	switch (itemType)
	{
	case itemSmallHeart: break;
	case itemWhip:
		if (whip) whip->upgradeWhipLv();
		collectingWhipTimer->Start();
		currentFrame = currentFrame - 1;
		if (currentFrame < 0) currentFrame = 0;
		forceRenderFrame = currentFrame;
		break;
	default: subWeapon = SubWeaponFactory::getInstance()->getSubWeapon(itemDagger);
		break;
	}

	item->setEnable(false);
}

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
		animId = ANIM_WALK;
	}
	else if (currentState == sitting)
	{
		if (isReleaseSitButton)
		{
			standUp();
		}
		animId = ANIM_SIT;
	}
	else if (currentState == jumping)
	{
		// will sit if get enough jump enough high;
		if (vy > 0.15)
			animId = ANIM_IDLE;
		else
			animId = ANIM_SIT;
	}
	else if (currentState == hitting)
	{
		// set hitting anim
		animId = ANIM_HITTING;
		// check and process if animation hitting is done
		if (animations[animId])
		{
			if (animations[animId]->isDone())
			{
				whip->refreshAnim();
				animations[animId]->refresh();
				stand();
				animId = ANIM_IDLE;
			}
		}
	}
	else if (currentState == hittingWhenSitting)
	{
		animId = ANIM_HITTING_WHEN_SIT;
		if (animations[animId])
		{
			const auto frame = animations[animId]->getCurrentFrame();
			if (animations[animId]->isDone())
			{
				whip->refreshAnim();
				isHitting = false;
				setState(sitting);
				animations[animId]->refresh();
				animId = ANIM_SIT;
			}
		}
	}
	else if (currentState == State::throwing)
	{
		animId = ANIM_HITTING;
		if (animations[animId])
		{
			const auto frame = animations[animId]->getCurrentFrame();
			if (frame == 2) throwSubWeapon();
			else if (animations[animId]->isDone())
			{
				animations[animId]->refresh();
				stand();
			}
		}
	}
	else if (currentState == State::throwingWhenSitting)
	{
		animId = ANIM_HITTING_WHEN_SIT;
		if (animations[animId])
		{
			const auto frame = animations[animId]->getCurrentFrame();
			if (frame == 2) throwSubWeapon();
			else if (animations[animId]->isDone())
			{
				animations[animId]->refresh();
				setState(State::sitting);
				isThrowing = false;
				animId = ANIM_IDLE;
			}
		}
	}
	else
	{
		animId = ANIM_IDLE;
	}
}

void Simon::setState(int state)
{
	GameObject::setState(state);
}

void Simon::move(int side)
{
	setFaceSide(side);
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
	vx = 0;
	vy = 0;
	isThrowing = true;
	setState(State::throwing);
}

void Simon::throwingWhenSitting()
{
	vx = 0;
	vy = 0;
	isThrowing = true;
	setState(State::throwingWhenSitting);
}

void Simon::throwSubWeapon()
{
	subWeapon = subWeaponFactory->getSubWeapon(itemDagger);
	subWeapon->setFaceSide(faceSide);
	const auto subX = faceSide == FaceSide::left ? x - SIM_HIT_W + 10 : x + SIM_HIT_W - 10;
	const auto subY = y;

	subWeapon->setPosition(subX, subY);
	subWeapon->setEnable();
	StageManager::getInstance()->add(subWeapon);
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
			standUp();
	}
}

void Simon::handleOnKeyPress(BYTE* states)
{
	auto game = Game::getInstance();

	if (isHitting || isThrowing || isCollectingWhip) return;
	if (currentState == sitting) return;
	if (currentState == jumping) return;


	if (game->isKeyDown(DIK_RIGHT))
		move(FaceSide::right);
	else if (game->isKeyDown(DIK_LEFT))
		move(FaceSide::left);
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

bool Simon::isDoingImportantAnim()
{
	return isHitting || isThrowing || isCollectingWhip;
}

void Simon::handleOnKeyDown(int keyCode)
{
	if (isDoingImportantAnim()) return;

	switch (keyCode)
	{
	case DIK_SPACE:
		if (previousState != jumping && isInGround && !isHitting)
			jump();
		break;
	case DIK_LCONTROL:
		isReleaseSitButton ? hit() : hitWhenSitting();
		break;
	case DIK_A:
		isReleaseSitButton ? throwing() : throwingWhenSitting();
		break;
	case DIK_DOWN:
		if (isInGround
			&& currentState != sitting
			&& currentState != jumping
		)
		{
			isReleaseSitButton = false;
			sit();
		}
		break;
	default: ;
	}
}

Simon::~Simon()
= default;
