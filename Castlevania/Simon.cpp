#include "Simon.h"
#include "StageManager.h"
#include "SubWeaponDagger.h"
#include "ObjectChangeStage.h"
#include "Enemy.h"

auto const subWeaponFactory = SubWeaponFactory::getInstance();

Simon::Simon()
{
	init();
}

void Simon::init()
{
	whip = new Whip();
	subWeaponType = -1;
	climbDirection = ClimbNull;
	staringStatus = pause;
	whip->setPosition(x, y);

	isInGround = false;
	isReleaseSitButton = true;
	Simon::initAnim();
	animId = ANIM_IDLE;
	//gravity = 0;
	gravity = SIMON_GRAVITY;
	type = simon;
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

void Simon::update(DWORD dt, const vector<MapGameObjects>& maps)
{
	if (forceDead) return;
	updateAutoWalk(dt);
	doAutoClimb(dt);
	checkIfFalling(dt);

	processDeflectEffect();
	updateChangingStageEffect();
	GameObject::update(dt);

	updateRGB();
	processDeathEffect();
	checkCollision(dt, maps);
	updateAnimId();
	// simple fall down
	updateGravity(gravity);
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

void Simon::checkCollision(DWORD dt, const vector<MapGameObjects>& maps)
{
	for (auto map : maps)
	{
		switch (map.id)
		{
		case boundary:
			if (state != climbing)checkCollisionWithBoundary(dt, map.objs);
			else updatePosWhenNotCollide();
			break;
		case item: checkCollisionWithItems(dt, map.objs);
			break;
		case canHitObjs: updateWhip(dt, map.objs);
			break;
		case obChangeStage: checkCollisionWithObChangeStage(dt, map.objs);
			break;
		case enemy:
			checkCollisionWithEnemy(dt, map.objs);
			updateWhip(dt, map.objs);
			break;
		case stair:
			listStairs = map.objs;
			checkCollisionWithStair(listStairs);
			break;
		default: DebugOut(L"[WARNING] unknown obj to check collision with id %d!\n", map.id);
		}
	}
}

bool Simon::canAutoWalkWithDistance()
{
	return autoWalkDistance > 0;
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

void Simon::doChangeStageEffect()
{
	if (isChangingStage())return;
	timerChangeStage->start();
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
		{
			collidedStair = stair;
		}

		if (state == climbing && collidedStair)
		{
			auto isEndOfStairUp = collidedStair->getStairType() == StairEnd && climbDirection == ClimbUp;
			auto isEndOfStairDown = collidedStair->getStairType() == StairStartUp && climbDirection == ClimbDown;

			if (isEndOfStairDown || isEndOfStairUp)
			{
				setState(idle);
				removeAllVelocity();
				removeAutoclimbDistance();
				animId = ANIM_IDLE;
			}
		}
	}
}

bool Simon::canAutoClimb()
{
	return stairDxRemain > 0 && stairDyRemain > 0;
}

void Simon::doAutoClimb(DWORD dt)
{
	if (!canAutoClimb())
		return;
	if (state == climbing)
	{
		const auto climbSpeed = SIM_CLIMB_VELOCITY;
		vx = climbSpeed * faceSide;
		vy = static_cast<float>(climbDirection) * -climbSpeed;
		stairDxRemain -= float(dt) * climbSpeed;
		stairDyRemain -= float(dt) * climbSpeed;
	}
}

bool Simon::isAutoWalking()
{
	return timerAutoWalk->isRunning() || canAutoWalkWithDistance();
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

		// block
		updatePosInTheMomentCollideAndRemoveVelocity(minTx, minTy, nx, ny);

		for (auto& i : coEventsResult)
		{
			const auto object = (i->obj);
			const auto boundary = dynamic_cast<Boundary*>(object);
			if (boundary->getBoundaryType() == BoundaryNormal)
				processCollisionWithBoundaryByX(minTx, nx, boundary);
			if (boundary->getBoundaryType() == BoundaryGround)
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

void Simon::powerUpWhip(bool upgrade)
{
	if (whip) whip->upgradeWhipLv(upgrade);
	if (isHitting) return;

	if (isPowering()) return;
	timerPowering->start();
	vx = 0;
}

void Simon::reset()
{
	resetState();
	x = initPos.x;
	y = initPos.y;
	timerPowering->stop();
	timerDeflect->stop();
	timerChangeStage->stop();
	timerThrowing->stop();
	timerUntouchable->stop();
	timerAutoWalk->stop();
	autoWalkDistance = -1;
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

void Simon::updateWhip(DWORD dt, vector<GameObject*>* objs)
{
	whip->update(dt, x, y, objs, state);
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

void Simon::processCollisionWithGround(float minTy, float ny)
{
	if (ny == CDIR_BOTTOM)
	{
		vx = 0;
		vy = 0;
		isInGround = true;
		if (state == jumping)
			standUp();
	}
}

void Simon::processCollisionWithBoundaryByX(float minTx, float ny, Boundary* boundary)
{
	auto type = boundary->getBoundaryType();
}

void Simon::updateAnimId()
{
	if (isAutoWalking() || isChangingStage() || isDying())
	{
		animId = ANIM_WALK;
		GameObject::updateAnimId();
		return;
	}
	int frame;
	switch (state)
	{
	case climbing:
		animId = climbDirection == ClimbUp ? ANIM_UP_STAIR : ANIM_DOWN_STAIR;
		break;
	case walking: animId = ANIM_WALK;
		break;
	case sitting:
		if (isReleaseSitButton) standUp();
		animId = ANIM_SIT;
		break;
	case jumping:
		animId = (vy > SIM_VY_READY_TO_LAND) ? ANIM_IDLE : ANIM_SIT;
		break;
	case hitting:
		animId = ANIM_HITTING;
		// check and process if animation hitting is done
		if (animations[animId]->isDone())
		{
			whip->refreshAnim();
			animations[animId]->refresh();
			stand();
			animId = ANIM_IDLE;
		}
		break;
	case hittingWhenSitting:
		animId = ANIM_HITTING_WHEN_SIT;
		if (animations[animId]->isDone())
		{
			whip->refreshAnim();
			isHitting = false;
			setState(sitting);
			animations[animId]->refresh();
			animId = ANIM_SIT;
		}
		break;

	case hittingWhenClimbing:
		animId = ANIM_HIT_UP_STAIR;
		if (animations[animId]->isDone())
		{
			whip->refreshAnim();
			isHitting = false;
			setState(climbing);
			animations[animId]->refresh();
			animId = ANIM_UP_STAIR;
		}
		break;
	case State::throwing:
		animId = ANIM_HITTING;
		frame = animations[animId]->getCurrentFrame();
		if (frame == 2) throwSubWeapon();
		else if (animations[animId]->isDone())
		{
			animations[animId]->refresh();
			stand();
		}
		break;
	case State::throwingWhenSitting:
		animId = ANIM_HITTING_WHEN_SIT;
		frame = animations[animId]->getCurrentFrame();
		if (frame == 2) throwSubWeapon();
		else if (animations[animId]->isDone())
		{
			animations[animId]->refresh();
			setState(sitting);
			isThrowing = false;
			animId = ANIM_IDLE;
		}
		break;
	default:
		animId = ANIM_IDLE;
	}
	GameObject::updateAnimId();
}

void Simon::setState(int state)
{
	GameObject::setState(state);
}

bool Simon::isCollidingWithStair()
{
	collidedStair = nullptr;
	checkCollisionWithStair(listStairs);
	return collidedStair != nullptr;
}

void Simon::standAfterClimbStair()
{
	removeAutoclimbDistance();
	staringStatus = pause;
	stand();
	removeAllVelocity();
	gravity = SIMON_GRAVITY;
}

void Simon::setClimbStairInfo(int direction)
{
	if (collidedStair->getStairType() == StairEnd)
	{
		standAfterClimbStair();
		return;
	}

	setState(climbing);
	faceSide = collidedStair->getFaceSide() * direction;
	const auto nextPos = collidedStair->getNextPos();
	stairDxRemain = nextPos.x;
	stairDyRemain = nextPos.y;
	gravity = 0;
	staringStatus = onGoing;
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

bool Simon::canThrow()
{
	const auto isHaveEnoughEnergy = energy > 0;
	return isHaveEnoughEnergy && isHaveSubWeapon() && timerThrowing->isTimeUp();
}

void Simon::throwing(int type)
{
	if (!canThrow()) return;
	stopMoveWhenHitting();
	isThrowing = true;
	setState(type);
}

void Simon::throwSubWeapon()
{
	if (!subWeapon && isHaveSubWeapon())
		generateSubWeapon();
	else
	{
		if (!isHaveSubWeapon() || subWeapon->IsActive() || subWeapon->IsEnable() || !timerThrowing->isTimeUp()) return;
		generateSubWeapon();
	}
}

void Simon::generateSubWeapon()
{
	loseEnergy();
	subWeapon = subWeaponFactory->getSubWeapon(subWeaponType, getFaceSide());
	const auto width = getBoundingBox().r - getBoundingBox().l;
	const auto subX = getFaceSide() == Side::SideLeft ? x - width + 10 : x + width;
	const auto subY = y;

	subWeapon->setInitPos({subX, subY});
	subWeapon->setPosition(subX, subY);
	subWeapon->setEnable();
	StageManager::getInstance()->add(subWeapon);
	timerThrowing->start();
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

void Simon::upgradeWhipLv(bool up) const
{
	whip->upgradeWhipLv(up);
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

	if (isDoingImportantAnim()) return;
	if (state == sitting) return;
	if (state == jumping) return;

	if (game->isKeyDown(DIK_RIGHT))
	{
		if (state == climbing)
		{
			if (collidedStair)
				climbStair(ClimbUp * collidedStair->getFaceSide());
		}
		else move(Side::SideRight);
	}
	else if (game->isKeyDown(DIK_LEFT))
	{
		if (state == climbing)
		{
			if (collidedStair)
				climbStair(ClimbDown * collidedStair->getFaceSide());
		}
		else move(Side::SideLeft);
	}
	else if (game->isKeyDown(DIK_DOWN))
	{
		isReleaseSitButton = false;
		if (collidedStair && collidedStair->getStairType() == StairStartDown)
			climbStair(ClimbDown);
		else if (state != climbing) sit();
		else climbStair(ClimbDown);
	}
	else if (game->isKeyDown(DIK_UPARROW))
	{
		auto result = climbStair(ClimbUp);
		if (!result) stand();
	}
	else if (game->isKeyDown(DIK_DOWNARROW))
	{
		auto result = climbStair(ClimbDown);
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
	if (isDoingImportantAnim()) return;

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
			auto hittingType = hitting;
			if (!isReleaseSitButton && !isJumpingHit) hittingType = hittingWhenSitting;
			throwing(hittingType);
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
	default: ;
	}
}

bool Simon::isDoingImportantAnim()
{
	return forceDead || isHitting || isThrowing || isPowering() || isAutoWalking() || isChangingStage() ||
		isDeflecting() || isDying() || canAutoClimb();
}

Box Simon::getBoundingBox()
{
	auto box = GameObject::getBoundingBoxBaseOnFile();
	// offset from rect sprite and bbox
	box.l = x + 10;
	box.r = box.l + SIM_WIDTH;
	return box;
}

Simon::~Simon()
= default;
