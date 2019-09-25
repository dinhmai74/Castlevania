#include "Simon.h"
#include "StageManager.h"
#include "SubWeaponDagger.h"
#include "ObjectChangeStage.h"

auto subWeaponFactory = SubWeaponFactory::getInstance();

Simon::Simon()
{
	init();
}

void Simon::init()
{
	whip = new Whip();
	subWeaponType = -1;
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
	addAnimation(ANIM_DEFLECT, "simon_deflect");
	addAnimation(ANIM_DEATH, "simon_death");
}

void Simon::render()
{
	if (forceDead)
	{
		animations[ANIM_DEATH]->render(faceSide, x, y);
		return;
	}
	if (isHitting && timerPowering->isTimeUp())
	{
		whip->setSide(faceSide);
		whip->render();
	}

	if (!timerPowering->isTimeUp())
	{
		animations[animId]->render(faceSide, x, y, currentFrame, alpha, r, g, b);
	}
	else animations[animId]->render(faceSide, x, y, alpha, r, g, b);

	currentFrame = animations[animId]->getCurrentFrame();
	preAnimId = animId;
}

void Simon::update(DWORD dt, const vector<MapGameObjects>& maps)
{
	if (forceDead) return;
	GameObject::update(dt);


	updateRGB();
	updateAutoWalk();
	updateChangingStageEffect();
	processDeflectEffect();
	processDeathEffect();

	checkCollision(dt, maps);
	updateAnimId();
	// simple fall down
	updateGravity(gravity);
}

bool Simon::updateLife(int val)
{
	life += val;
	if (life <= 0) {
		life = 0;
		return false;
	}
	return true;
}

bool Simon::updateHP(int val)
{
	hp += val;
	if (hp <= 0) {
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

void Simon::updateAutoWalk()
{
	if (isAutoWalking()) vx = faceSide * SIM_AUTO_WALK_VX;
}

void Simon::updateChangingStageEffect()
{
	if (isChangingStage()) {
		vx = faceSide * SIM_AUTO_WALK_VX;
		auto timeRunAlr = timerChangeStage->getTimeRunAlr();
		auto limedTime = timerChangeStage->getLimitedTime();
		if (limedTime - timeRunAlr <= 600) { alpha = 0; } // make simon invi
		startedChangeStage = true;
	}
	else
	{
		if (startedChangeStage)StageManager::getInstance()->nextStage(stageWillChangeTo);
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
		case obChangeStage: checkCollisionWithObChangeStage(dt, map.objs);
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
		if (isColliding(getBoundingBox(), e->getBoundingBox())) {
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

void Simon::doAutoWalk()
{
	if (isAutoWalking()) return;
	timerAutoWalk->start();
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
		updatePosInTheMomentCollide(minTx, minTy, nx, ny);

		for (auto& i : coEventsResult)
		{
			const auto object = (i->obj);
			const auto boundary = dynamic_cast<Boundary*>(object);
			if (nx != 0)
				processCollisionWithBoundaryByX(minTx, nx, boundary);
			if (ny != 0)
			{
				processCollisionWithGround(minTy, ny);
			}
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
}

void Simon::setHp(int val)
{
	hp = val;
}

void Simon::setEnegery(int val)
{
	energy = val;
}

void Simon::ForceDead()
{
	forceDead = true;
}

void Simon::updateWeaponAction(DWORD dt, vector<GameObject*>* objs)
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
	vy = 0;
	isInGround = true;
	if (state == jumping)
		standUp();
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
	vx = 0;
	setState(idle);
}

void Simon::stopMoveWhenHitting()
{
	if (state == jumping) return;
	vx = 0;
}

void Simon::hit()
{
	stopMoveWhenHitting();
	isHitting = true;
	whip->refreshAnim();
	setState(hitting);
}

void Simon::hitWhenSitting()
{
	stopMoveWhenHitting();
	isHitting = true;
	whip->refreshAnim();
	setState(hittingWhenSitting);
}

bool Simon::canThrow()
{
	const auto isHaveEnoughEnergy = energy > 0;
	return isHaveEnoughEnergy && isHaveSubWeapon() && timerThrowing->isTimeUp();
}

void Simon::throwing()
{
	if (!canThrow()) return;
	stopMoveWhenHitting();
	isThrowing = true;
	setState(State::throwing);
}

void Simon::throwingWhenSitting()
{
	if (!canThrow()) return;
	stopMoveWhenHitting();
	isThrowing = true;
	setState(State::throwingWhenSitting);
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
	subWeapon = subWeaponFactory->getSubWeapon(subWeaponType, faceSide);
	const auto width = getBoundingBox().r - getBoundingBox().l;
	const auto subX = faceSide == FaceSide::left ? x - width + 10 : x + width;
	const auto subY = y;

	subWeapon->setPosition(subX, subY);
	subWeapon->setEnable();
	StageManager::getInstance()->add(subWeapon);
	timerThrowing->start();
}

void Simon::resetState()
{
	isHitting = false;
	isThrowing = false;
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

	if (isDoingImportantAnim()) return;
	if (state == sitting) return;
	if (state == jumping) return;


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
	return forceDead || isHitting || isThrowing || isPowering() || isAutoWalking() || isChangingStage() || isDeflecting() || isDying();
}

void Simon::handleOnKeyDown(int keyCode)
{
	if (isDoingImportantAnim()) return;

	auto isJumpingHit = false;
	switch (keyCode)
	{
	case DIK_SPACE:
		if (previousState != jumping && isInGround && !isHitting)
			jump();
		break;
	case DIK_LCONTROL:
		if (state == jumping) isJumpingHit = true;
		(isReleaseSitButton || isJumpingHit) ? hit() : hitWhenSitting();
		break;
	case DIK_A:
		isReleaseSitButton ? throwing() : throwingWhenSitting();
		break;

	case DIK_DOWN:
		if (isInGround
			&& state != sitting
			&& state != jumping
			)
		{
			isReleaseSitButton = false;
			sit();
		}
		break;
	default:;
	}
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
