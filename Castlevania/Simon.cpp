#include "Simon.h"
#include "StageManager.h"
#include "SubWeaponDagger.h"
#include "ObjectChangeStage.h"
#include "Enemy.h"
#include "Door.h"
#include "ForceIdleSim.h"

auto const subWeaponFactory = SubWeaponFactory::getInstance();

Simon::Simon()
{
	init();
}

/*----------------- init and render  -----------------*/

void Simon::init()
{
	whip = new Whip();
	subWeaponType = -1;
	climbDirection = ClimbNull;
	staringStatus = pause;
	goThroughDoorStatus = nope;
	whip->setPosition(x, y);

	isInGround = false;
	isReleaseSitButton = true;
	Simon::initAnim();
	setAnimId(ANIM_IDLE);
	//gravity = 0;
	gravity = SIMON_GRAVITY;
	type = OBSimon;
}

void Simon::initAnim()
{
	addAnimation(ANIM_IDLE, "simon_stand_ani");
	addAnimation(ANIM_WALK, "simon_walk_ani");
	addAnimation(ANIM_SIT, "simon_sit_ani");
	addAnimation(ANIM_HITTING, "simon_hitstand_ani");
	addAnimation(ANIM_HITTING_WHEN_SIT, "simon_hitsit_ani");
	addAnimation(ANIM_DEFLECT, "simon_deflect_ani");
	addAnimation(ANIM_DEATH, "simon_death_ani");
	addAnimation(ANIM_UP_STAIR, "simon_up_stair_ani");
	addAnimation(ANIM_DOWN_STAIR, "simon_down_stair_ani");
	addAnimation(ANIM_HIT_UP_STAIR, "simon_hit_up_stair_ani");
	addAnimation(ANIM_HIT_DOWN_STAIR, "simon_hit_down_stair_ani");
}

void Simon::render()
{
	if (forceDead)
	{
		animations[ANIM_DEATH]->render(getFaceSide(), x, y);
		return;
	}
	renderWhip();

	auto const forceRenderFrame = !timerPowering->isTimeUp() || forceRenderStaringAnimStand();
	if (forceRenderFrame)
		animations[animId]->render(getFaceSide(), x, y, currentFrame, alpha, r, g, b);
	else animations[animId]->render(getFaceSide(), x, y, alpha, r, g, b);

	currentFrame = animations[animId]->getCurrentFrame();
	preAnimId = animId;
}

void Simon::renderWhip()
{
	if (isHitting && timerPowering->isTimeUp())
	{
		whip->setSide(getFaceSide());
		whip->render();
	}
}

bool Simon::forceRenderStaringAnimStand()
{
	if (state == climbing)
	{
		if (staringStatus == pause && !canAutoClimb())
		{
			currentFrame = 0;
			return true;
		}
	}

	return false;
}

/*----------------- update  -----------------*/

void Simon::update(DWORD dt, const vector<MapGameObjects>& maps)
{
	if (forceDead) return;

	if (state == climbing) canDeflect = false;
	else canDeflect = true;
	updateAutoWalk(dt);
	doAutoClimb(dt);

	checkIfFalling(dt);
	processDeflectEffect();
	updateChangingStageEffect();
	updateCameraWhenGoThroughDoor(dt);
	GameObject::update(dt);

	updateRGB();
	processDeathEffect();
	checkCollision(dt, maps);
	updateAnimId();
	// simple fall down
	updateGravity(gravity);

	checkBoundary();
}

void Simon::updateCameraWhenGoThroughDoor(DWORD dt)
{
	if (!collidedDoor) return;
	auto cam = Camera::getInstance();
	switch (goThroughDoorStatus)
	{
	case ThroughDoorStarted:
		if (!cam->isMoving())
		{
			goThroughDoorStatus += 1;
		}
		break;
	case ThroughDoorOpening:
		if (collidedDoor->isOpened()) goThroughDoorStatus += 1;
		this->collidedDoor->openDoor();
	case ThroughStartWalk:
		doAutoWalkWithDistance(150);
		goThroughDoorStatus += 1;
		break;
	case ThroughWalkDone:
		if (autoWalkDistance <= 0)
			goThroughDoorStatus += 1;
		break;
	case ThroughDoorStartClose:
		collidedDoor->closeDoor();
		goThroughDoorStatus += 1;
		break;
	case ThroughDoorClosing:
		if (collidedDoor->getState() == DoorCloseIdle)
			goThroughDoorStatus += 1;
		break;
	case ThroughDoorClosed:
		goThroughDoorStatus += 1;
		break;
	case ThroughDone:
		goThroughDoorStatus = nope;
		moveCam(collidedDoor->getMoveCamDistance());
		collidedDoor = nullptr;
		break;
	default:;
	}
}

bool Simon::updateLife(int val)
{
	life += val;
	if (life <= 0)
	{
		life = 0;
		return false;
	}
	return true;
}

bool Simon::updateHP(int val)
{
	hp += val;
	if (hp <= 0)
	{
		hp = 0;
		return false;
	}

	if (hp > SIM_MAX_HP) hp = SIM_MAX_HP;
	return true;
}

void Simon::updateRGB()
{
	if (!timerPowering->isTimeUp())
	{
		r = g + 80;
		g = 100 + rand() % 150;
		b = g + 35;
	}
	else
	{
		r = g = b = 255;
		isCollectingWhip = false;
		timerPowering->stop();
	}
}

void Simon::updateAutoWalk(DWORD dt)
{
	if (timerAutoWalk->isRunning())
		vx = vxAutoWalk * float(faceSide);
	else if (canAutoWalkWithDistance())
	{
		vx = vxAutoWalk * float(faceSide);
		autoWalkDistance -= vxAutoWalk * dt;
	}
	else if (staringStatus == ready) setClimbStairInfo(climbDirection);
}

void Simon::updateChangingStageEffect()
{
	if (isChangingStage())
	{
		vx = getFaceSide() * SIM_AUTO_WALK_VX;
		startedChangeStage = true;
	}
	else
	{
		if (startedChangeStage)
		{
			StageManager::getInstance()->nextStage(stageWillChangeTo);
			startedChangeStage = false;
		}
	}
}

void Simon::updateWhip(DWORD dt, vector<GameObject*>* objs)
{
	whip->update(dt, x, y, objs, state, climbDirection);
}

void Simon::doChangeStageEffect(DWORD duration)
{
	if (isChangingStage())return;
	timerChangeStage->setLimitedTime(duration);
	timerChangeStage->start();
}

void Simon::doAutoClimb(DWORD dt)
{
	if (!canAutoClimb())
	{
		return;
	}
	if (state == climbing)
	{
		const auto climbSpeed = SIM_CLIMB_VELOCITY;
		vx = climbSpeed * faceSide;
		vy = static_cast<float>(climbDirection) * -climbSpeed;
		stairDxRemain -= float(dt) * climbSpeed;
		stairDyRemain -= float(dt) * climbSpeed;
	}
}

void Simon::checkIfFalling(DWORD dt)
{
	if (vy != 0) isInGround = false;
}

void Simon::doAutoWalk(DWORD dt, float vx)
{
	if (timerAutoWalk->isRunning()) return;
	timerAutoWalk->setLimitedTime(dt);
	vxAutoWalk = vx;
	timerAutoWalk->start();
}

void Simon::doAutoWalkWithDistance(float distance, float vx)
{
	if (distance < 0) faceSide = SideLeft;
	else faceSide = SideRight;
	vxAutoWalk = vx;
	autoWalkDistance = fabs(distance);
}

void Simon::processDeathEffect()
{
	if (isDying())
	{
		vx = 0;
		startDying = true;
	}
	else if (startDying)
	{
		startDying = false;
		StageManager::getInstance()->descreaseLife();
	}
}

void Simon::checkBoundary()
{
	if (this->goThroughDoorStatus != nope) return;
	auto cam = Camera::getInstance();
	auto limit = cam->getLimitX();
	auto offset = 10;
	if (x + offset <= limit.min) x = limit.min - offset;

	auto width = (getBoundingBox().r - getBoundingBox().l);
	if (x + width >= limit.max) x = limit.max - width;
}

/*----------------- check collision  -----------------*/

void Simon::checkCollision(DWORD dt, const vector<MapGameObjects>& maps)
{
	for (auto map : maps)
	{
		switch (map.id)
		{
		case OBBoundary:
			if (state != climbing) checkCollisionWithBoundary(dt, map.objs);
			else updatePosWhenNotCollide();
			break;
		case OBItem: checkCollisionWithItems(dt, map.objs);
			break;
		case OBCanHitObjs: updateWhip(dt, map.objs);
			break;
		case OBChangeStage: checkCollisionWithObChangeStage(dt, map.objs);
			break;
		case OBEnemy:
			checkCollisionWithEnemy(dt, map.objs);
			updateWhip(dt, map.objs);
			break;
		case OBStair:
			listStairs = map.objs;
			checkCollisionWithStair(listStairs);
			break;
		case OBForceIdleSim:
			checkCollisionWithForceIdleSim(dt, map.objs);
			break;
		default: DebugOut(L"[WARNING] unknown obj to check collision with id %d!\n", map.id);
		}
	}
}

void Simon::checkCollisionWithObChangeStage(DWORD dt, vector<GameObject*>* objs)
{
	for (auto i = 0; i < objs->size(); i++)
	{
		auto e = objs->at(i);
		auto objectChangeStage = dynamic_cast<ObjectChangeStage*>(e);
		if (isColliding(getBoundingBox(), e->getBoundingBox()) && !isChangingStage())
		{
			stageWillChangeTo = objectChangeStage->NextStage();
			doChangeStageEffect();
		}
		break;
	}
}

void Simon::checkCollisionWithStair(vector<GameObject*>* objs)
{
	collidedStair = nullptr;
	for (auto obj : *objs)
	{
		const auto stair = dynamic_cast<Stair*>(obj);
		auto box = getBoundingBox();
		box.t = box.b - 5;
		if (isColliding(box, obj->getBoundingBox()) && stair)
			collidedStair = stair;
		if (state == climbing && collidedStair)
		{
			auto isEndOfStairUp = collidedStair->getStairType() == StairEnd && climbDirection == ClimbUp;
			auto isEndOfStairDown = collidedStair->getStairType() == StairStartUp && climbDirection == ClimbDown;

			if (isEndOfStairDown) collidedStair = nullptr;
			if (isEndOfStairUp) forceStopClimb(ClimbUp);
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
	if (coEvents.empty()) updatePosWhenNotCollide();
	else
	{
		float minTx;
		float minTy;
		float nx = 0;
		float ny;
		filterCollision(coEvents, coEventsResult, minTx, minTy, nx, ny);

		auto isCollideX = false;
		auto isCollideY = false;
		auto isCollideDoor = false;
		// block

		for (auto& i : coEventsResult)
		{
			const auto object = (i->obj);
			const auto boundary = dynamic_cast<Boundary*>(object);
			if (boundary)
			{
				auto type = boundary->getBoundaryType();
				switch (type)
				{
				case BoundaryNormal:
					isCollideX = processCollisionWithBoundaryByX(minTx, nx, boundary);
					break;
				case BoundaryGround:
					isCollideY = processCollisionWithGround(minTy, ny);
					break;
				default:;
				}
			}

			const auto door = dynamic_cast<Door*>(object);
			if (door)
				isCollideDoor = processCollisionWithDoor(minTx, nx, door);
		}
		auto updatedX = false;
		auto updatedY = false;

		if (isCollideX)
		{
			blockX(minTx, nx);
			updatedX = true;
		}
		if (isCollideY)
		{
			blockY(minTy, ny);
			updatedY = true;
		}
		if (isCollideDoor)
		{
			blockX(minTx, nx);
			updatedX = true;
		}

		if (!updatedX) x += dx;
		if (!updatedY) y += dy;
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

void Simon::checkCollisionWithEnemy(DWORD dt, vector<GameObject*>* objs)
{
	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;
	coEvents.clear();

	calcPotentialCollisions(objs, coEvents);
	calcPotentialCollisionsAABB(objs, coEvents);

	// no collison => check case inside
	if (!coEvents.empty())
	{
		float minTx;
		float minTy;
		float nx = 0;
		float ny;
		filterCollision(coEvents, coEventsResult, minTx, minTy, nx, ny);

		for (auto& i : coEventsResult)
		{
			const auto object = (i->obj);
			const auto enemy = dynamic_cast<Enemy*>(object);
			if (enemy)
			{
				getHurt(nx, ny, enemy->getDmg());
			}
		}
	}

	for (auto& coEvent : coEvents) delete coEvent;
}

bool Simon::processCollisionWithDoor(float minTx, float nx, Door* door)
{
	auto result = false;
	if (nx == door->getFaceSide() && !door->isOpenning())
	{
		setState(idle);
		auto cam = Camera::getInstance();
		cam->setLimitX(door->getNextCameraLimit());
		moveCam(door->getMoveCamDistance());
		collidedDoor = door;
		goThroughDoorStatus = ThroughDoorStarted;
	}

	if (nx != 0 && !door->isOpenning()) result = true;

	return result;
}

void Simon::processCollisionWithItem(Item* item)
{
	const auto itemType = item->getItemType();
	const auto itemHeart = dynamic_cast<ItemHeart*>(item);
	switch (itemType)
	{
	case itemSmallHeart:
	case itemBigHeart:
		energy += itemHeart->getEnergy();
		break;

	case itemWhip:
		powerUpWhip();
		break;
	default:
		subWeaponType = itemDagger;
		break;
	}

	item->setActive(false);
}

bool Simon::processCollisionWithGround(float minTy, float ny)
{
	if (ny == CDIR_BOTTOM)
	{
		vx = 0;
		vy = 0;
		isInGround = true;
		if (state == jumping)
			standUp();
		return true;
	}

	return false;
}

bool Simon::processCollisionWithBoundaryByX(float minTx, float nx, Boundary* boundary)
{
	if (nx == 0) return false;
	return true;
}

void Simon::checkCollisionWithForceIdleSim(DWORD dt, vector<GameObject*>* objs)
{
	for (auto obj : *objs)
	{
		auto force = dynamic_cast<ForceIdleSim*>(obj);
		if (force && isColliding(getBoundingBox(), force->getBoundingBox())) {
			auto res = forceStopClimb(force->getDirection());
			if (res)
				y = force->getNextY();
		}
	}
}

/*----------------- actions  -----------------*/

void Simon::standAfterClimbStair()
{
	staringStatus = pause;
	forceStopClimb(climbDirection);
	setFaceSide(climbDirection);
	gravity = SIMON_GRAVITY;
}

bool Simon::climbStair(int direction)
{
	if (!isCollidingWithStair())
	{
		vx = 0;
		vy = 0;
		staringStatus = pause;
		return false;
	}
	if (staringStatus == ready || stairDxRemain > 0 || stairDyRemain > 0) return false;

	if (collidedStair->getStairType() == StairStartUp && direction != ClimbUp) return false;
	if (collidedStair->getStairType() == StairStartDown && direction != ClimbDown) return false;

	if (state != climbing)
	{
		const auto collidePos = collidedStair->getPosition();
		const auto finalStandPos = collidePos.x - (getBoundingBox().l - x) - 6 * collidedStair->getFaceSide();
		doAutoWalkWithDistance(finalStandPos - x);
		staringStatus = ready;
	}
	else
	{
		setClimbStairInfo(direction);
	}
	climbDirection = direction;

	return true;
}

void Simon::move(int side)
{
	setFaceSide(side);
	setState(walking);
	vx = SIMON_VX * getFaceSide();
	gravity = SIMON_GRAVITY;
}

void Simon::jump()
{
	setState(jumping);
	vy = -SIMON_VJUMP;
	isInGround = false;
	gravity = SIMON_GRAVITY;
}

void Simon::sit()
{
	if (!isInGround) return;
	setState(sitting);
	vx = 0;
	vy = 0;
	gravity = SIMON_GRAVITY;
}

void Simon::stand()
{
	resetState();
	setState(idle);
}

void Simon::standUp()
{
	resetState();
	setState(idle);
}

void Simon::stopMoveWhenHitting()
{
	if (state == jumping) return;
	vx = 0;
}

void Simon::hit(int type)
{
	stopMoveWhenHitting();
	isHitting = true;
	whip->refreshAnim();
	setState(type);
}

void Simon::doThrow(int type)
{
	if (!canThrow()) return;
	stopMoveWhenHitting();
	isThrowing = true;
	setState(type);
}

void Simon::throwSubWeapon()
{
	if (canThrow())
		generateSubWeapon();
}

void Simon::generateSubWeapon()
{
	loseEnergy();
	subWeapon = subWeaponFactory->getSubWeapon(subWeaponType, getFaceSide());
	const auto width = getBoundingBox().r - getBoundingBox().l;
	const auto subX = getFaceSide() == SideLeft ? x - width + 10 : x + width;
	const auto subY = state == throwingWhenSitting ? y + 15 : y;

	subWeapon->setInitPos({ subX, subY });
	subWeapon->setPosition(subX, subY);
	subWeapon->setEnable();
	StageManager::getInstance()->add(subWeapon);
	timerThrowing->start();
}

void Simon::handleOnKeyRelease(int KeyCode)
{
	if (KeyCode == DIK_DOWN)
	{
		isReleaseSitButton = true;
		if (isInGround && !isHitting && !isThrowing && state != climbing)
			standUp();
	}
}

void Simon::handleOnKeyPress(BYTE* states)
{
	auto game = Game::getInstance();

	if (shouldKeyboardDisable()) return;
	if (state == sitting) return;
	if (state == jumping) return;

	if (game->isKeyDown(DIK_RIGHT))
	{
		if (state == climbing)
		{
			if (collidedStair)
				climbStair(ClimbUp * collidedStair->getFaceSide());
		}
		else move(SideRight);
	}
	else if (game->isKeyDown(DIK_LEFT))
	{
		if (state == climbing)
		{
			if (collidedStair)
				climbStair(ClimbDown * collidedStair->getFaceSide());
		}
		else move(SideLeft);
	}
	else if (game->isKeyDown(DIK_DOWN))
	{
		isReleaseSitButton = false;
		auto result = false;
		if (collidedStair && collidedStair->getStairType() == StairStartDown)
			result = climbStair(ClimbDown);
		else if (state != climbing)
		{
			sit();
			result = true;
		}
		else result = climbStair(ClimbDown);
		if (!result) stand();
	}
	else if (game->isKeyDown(DIK_UPARROW))
	{
		auto result = climbStair(ClimbUp);
		if (!result) stand();
	}
	else if (state == climbing)
	{
		staringStatus = pause;
		vx = 0;
		vy = 0;
	}
	else
	{
		stand();
	}
}

void Simon::handleOnKeyDown(int keyCode)
{
	if (shouldKeyboardDisable()) return;

	auto isJumpingHit = false;
	switch (keyCode)
	{
	case DIK_SPACE:
		if (previousState != jumping && isInGround && !isHitting && state != climbing)
			jump();
		break;
	case DIK_LCONTROL:
	{
		if (state == jumping) isJumpingHit = true;
		auto hittingType = hitting;
		if (!isReleaseSitButton && !isJumpingHit) hittingType = hittingWhenSitting;
		else if (state == climbing) hittingType = hittingWhenClimbing;
		hit(hittingType);
		break;
	}
	case DIK_A:
	{
		if (state == jumping) isJumpingHit = true;
		auto hittingType = throwing;
		if (!isReleaseSitButton && !isJumpingHit) hittingType = throwingWhenSitting;
		else if (state == climbing) hittingType = throwingWhenClimbing;
		this->doThrow(hittingType);
		break;
	}

	case DIK_DOWN:
		if (isInGround
			&& state != sitting
			&& state != jumping
			)
		{
			isReleaseSitButton = false;
			if (state != climbing) sit();
			else climbStair(ClimbDown);
		}
		break;
	default:;
	}
}

/*----------------- utilities  -----------------*/

void Simon::updateAnimId()
{
	if (isAutoWalking() || isChangingStage() || isDying())
	{
		setAnimId(ANIM_WALK);
		GameObject::updateAnimId();
		return;
	}
	int frame;
	switch (state)
	{
	case climbing:
		setAnimId(climbDirection == ClimbUp ? ANIM_UP_STAIR : ANIM_DOWN_STAIR);
		break;
	case walking: setAnimId(ANIM_WALK);
		break;
	case sitting:
		if (isReleaseSitButton) standUp();
		setAnimId(ANIM_SIT);
		break;
	case jumping:
		setAnimId((vy > SIM_VY_READY_TO_LAND) ? ANIM_IDLE : ANIM_SIT);
		break;
	case hitting:
		setAnimId(ANIM_HITTING);
		// check and process if animation hitting is done
		if (animations[animId]->isDone())
		{
			refreshHitAnim();
		}
		break;
	case hittingWhenSitting:
		setAnimId(ANIM_HITTING_WHEN_SIT);
		if (animations[animId]->isDone())
			refreshHitAnim(sitting, ANIM_SIT);
		break;

	case hittingWhenClimbing:
	{
		auto const anim = climbDirection == ClimbUp ? ANIM_HIT_UP_STAIR : ANIM_HIT_DOWN_STAIR;
		setAnimId(anim);
		if (animations[animId]->isDone())
			refreshHitAnim(climbing, anim);
	}
	break;
	case throwing:
		setAnimId(ANIM_HITTING);
		frame = animations[animId]->getCurrentFrame();
		if (frame == 2) throwSubWeapon();
		else if (animations[animId]->isDone())
			refreshHitAnim();
		break;
	case throwingWhenSitting:
		setAnimId(ANIM_HITTING_WHEN_SIT);
		frame = animations[animId]->getCurrentFrame();
		if (frame == 2) throwSubWeapon();
		else if (animations[animId]->isDone())
			refreshHitAnim(sitting, ANIM_SIT);
		break;
	case throwingWhenClimbing:
	{
		auto const anim = climbDirection == ClimbUp ? ANIM_HIT_UP_STAIR : ANIM_HIT_DOWN_STAIR;
		setAnimId(anim);
		frame = animations[animId]->getCurrentFrame();
		if (frame == 2) throwSubWeapon();
		else if (animations[animId]->isDone())
			refreshHitAnim(climbing, anim);
	}
	break;
	default:
		setAnimId(ANIM_IDLE);
	}
	GameObject::updateAnimId();
}

void Simon::refreshHitAnim(int stateAfterHit, int animAfterHit)
{
	whip->refreshAnim();
	isHitting = false;
	isThrowing = false;
	animations[animId]->refresh();
	setState(stateAfterHit);
	setAnimId(animAfterHit);
}

void Simon::setState(int state)
{
	GameObject::setState(state);
}

Box Simon::getBoundingBox()
{
	auto box = getBoundingBoxBaseOnFile();
	// offset from rect sprite and bbox
	box.l = x + 10;
	box.r = box.l + SIM_WIDTH;
	return box;
}

bool Simon::shouldKeyboardDisable()
{
	return forceDead || isHitting || isThrowing || isPowering() || isAutoWalking() || isChangingStage() ||
		isDeflecting() || isDying() || canAutoClimb() || forceDisable || goThroughDoorStatus != nope || Camera::
		getInstance()->isMoving();
}

void Simon::resetState()
{
	isHitting = false;
	isThrowing = false;
	whip->refreshAnim();
	whip->refreshState();
	animations[ANIM_HITTING]->refresh();
	animations[ANIM_HITTING_WHEN_SIT]->refresh();
}

void Simon::reset()
{
	resetState();
	x = initPos.x;
	y = initPos.y;
	gravity = SIMON_GRAVITY;
	timerPowering->stop();
	timerDeflect->stop();
	timerChangeStage->stop();
	timerThrowing->stop();
	timerUntouchable->stop();
	timerAutoWalk->stop();
	autoWalkDistance = -1;
	climbDirection = 0;
}

void Simon::setHp(int val)
{
	hp = val;
}

void Simon::setEnergy(int val)
{
	energy = val;
}

void Simon::setForceDead(bool val)
{
	forceDead = val;
}

void Simon::getHurt(int nx, int ny, int hpLose)
{
	if (isUntouching() || isDeflecting()) return;
	resetState();
	GameObject::getHurt(nx, ny, hpLose);
}

void Simon::upgradeWhipLv(bool up) const
{
	whip->upgradeWhipLv(up);
}

void Simon::powerUpWhip(bool upgrade)
{
	if (whip) whip->upgradeWhipLv(upgrade);
	if (isHitting) return;

	if (isPowering()) return;
	timerPowering->start();
	vx = 0;
}

bool Simon::isCollidingWithStair()
{
	collidedStair = nullptr;
	checkCollisionWithStair(listStairs);
	return collidedStair != nullptr;
}

bool Simon::canThrow()
{
	const auto isHaveEnoughEnergy = energy > 0;
	return isHaveEnoughEnergy && timerThrowing->isTimeUp() && isHaveSubWeapon();
}

void Simon::setClimbStairInfo(int direction)
{
	setState(climbing);
	faceSide = collidedStair->getFaceSide() * direction;
	const auto nextPos = collidedStair->getNextPos();
	stairDxRemain = nextPos.x;
	stairDyRemain = nextPos.y;
	gravity = 0;
	staringStatus = onGoing;
}

bool Simon::forceStopClimb(int direction)
{
	if (state != climbing || climbDirection != direction) return false;
	removeAllVelocity();
	removeAutoclimbDistance();
	collidedStair = nullptr;
	stand();
}

void Simon::removeAutoclimbDistance()
{
	stairDxRemain = -1;
	stairDyRemain = -1;
}

bool Simon::canAutoWalkWithDistance()
{
	return autoWalkDistance > 0;
}

bool Simon::canAutoClimb()
{
	return stairDxRemain > 0 && stairDyRemain > 0;
}

bool Simon::isAutoWalking()
{
	return timerAutoWalk->isRunning() || canAutoWalkWithDistance();
}

void Simon::moveCam(float distance)
{
	auto cam = Camera::getInstance();
	auto canMoveCam = !cam->isMoving();
	if (canMoveCam)
		cam->move(distance);
}

Simon::~Simon()
= default;
