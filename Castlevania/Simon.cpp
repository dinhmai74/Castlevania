#include "Simon.h"
#include "StageManager.h"
#include "SubWeaponDagger.h"
#include "ObjectChangeStage.h"
#include "Enemy.h"
#include "Door.h"
#include "ForceIdleSim.h"
#include "EnemyFactory.h"
#include "Bullet.h"
#include "ItemMoneyBag.h"

auto const subWeaponFactory = SubWeaponFactory::getInstance();

Simon::Simon() {
	init();
}

/*----------------- init and render  -----------------*/

void Simon::init() {
	whip = new Whip();
	subWeaponType = -1;
	whip->setPos(x, y);
	vxDeflect = 0.2f;
	vyDeflect = 0.09f;

	isInGround = false;
	isReleaseSitButton = true;
	isReleaseThrowButton = true;
	Simon::initAnim();
	setAnimId(ANIM_IDLE);
	canShotTimes = 1;
	//gravity = 0;
	gravity = SIMON_GRAVITY;
	initGravity = SIMON_GRAVITY;
	type = OBSimon;
}

void Simon::initAnim() {
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
	addAnimation(ANIM_IDLE_BACK, "simon_idle_back_ani");
}

void Simon::render() {
	if (state == StateNone) return;
	renderWhip();
	auto isPauseAnim = isStopAllAction || (staringStatus == pause && !isHitting);
	animations[animId]->render(faceSide, x, y, alpha, r, g, b, isPauseAnim, currentFrame);
	didSimonRender();
}

void Simon::didSimonRender() {
	currentFrame = animations[animId]->getCurrentFrame();
	preAnimId = animId;
}

void Simon::renderWhip() {
	if (isHitting && timerPowering->isTimeUp()) {
		whip->setSide(getFaceSide());
		whip->render();
	}
}
/*----------------- update  -----------------*/

// set current time dt, check some force state
bool Simon::shouldUpdate(DWORD dt) {
	this->dt = dt;
	// while climbing stair => do not defelect when get hit
	// canDeflect = state != climbing && staringStatus != ready;
	updateRGB();
	return !forceDead && !isStopAllAction;
}

void Simon::willUpdate() {
	updateAutoWalk();
	processDeflectEffect();
	processDeathEffect();
	if (state != climbing && state != hittingWhenClimbing && state != throwingWhenClimbing) updateGravity(dt);
}
// hieu ung tang mau' tu tu khi nhat vat pham
void Simon::autoUpdateHp() {
	if (timerAddHp->isTimeUpAndRunAlr() && hpWillAdd > 0) {
		hpWillAdd -= 1;
		addHP(1);
		timerAddHp->start();
	}
}

void Simon::update(DWORD dt, const vector<MapGameObjects>& maps) {
	if (!shouldUpdate(dt)) return;

	willUpdate();
	GameObject::update(dt);

	autoUpdateHp();
	checkCollision(dt, maps);
	updateAnimId();
	didUpdate();
}

void Simon::didUpdate() {
	checkOutOfBound();
}

void Simon::checkOutOfBound() {
	if (this->goThroughDoorStatus != nope) return;
	auto cam = Camera::getInstance();
	auto limit = cam->getLimitX();
	auto offset = 10;
	if (x + offset <= limit.min) x = limit.min - offset;

	auto width = (getBoundingBox().r - getBoundingBox().l);
	if (x + width + offset >= limit.max) x = limit.max - width - offset;
	//if (y > SCREEN_HEIGHT) setDeathByWater();
}

void Simon::updateRGB() {
	auto const stageManager = StageManager::getInstance();
	if (timerPowering->isRunning()) {
		r = g + 80;
		g = 100 + rand() % 150;
		b = g + 35;
		alpha = 255;
		stageManager->pauseGame();
	}
	else if (timerPowering->isTimeUpAndRunAlr()) {
		r = g = b = 255;
		isCollectingWhip = false;
		stageManager->pauseGame(false);
		timerPowering->stop();
	}
}

void Simon::updateAutoWalk() {
	if (canAutoWalkWithDistance()) {
		vx = vxAutoWalk * float(faceSide);
		autoWalkDistance -= vxAutoWalk * dt;
	}
}

void Simon::updateWhip(DWORD dt, vector<GameObject*>* objs) {
	whip->update(dt, x, y, objs, state, climbDirection);
}

void Simon::doAutoWalkWithDistance(float distance, float vx) {
	if (distance < 0) faceSide = SideLeft;
	else faceSide = SideRight;
	vxAutoWalk = vx;
	autoWalkDistance = fabs(distance);
}

void Simon::doAutoWalkWithDistance(float distance, float vx, int newState, int newFaceSide) {
	doAutoWalkWithDistance(distance, vx);

	stateAfterAutoWalk = newState;
	nxAfterAutoWalk = newFaceSide;
}

void Simon::processDeathEffect() {
	if (isDying()) {
		vx = 0;
		startDying = true;
	}
	else if (startDying) {
		startDying = false;
		StageManager::getInstance()->descreaseLife();
	}
}

/*----------------- check collision  -----------------*/

void Simon::checkCollision(DWORD dt, const vector<MapGameObjects>& maps) {
	// reason to check collision separate
	// sweptAABB: we only can detect 2 side of collision (x,y) 
	// in some case, simon got hurt from the sky ( y<0) but he collide with ground (y>0) 
	// and they happen in the same time, so we can't detect the collision in that case
	for (auto map : maps) {
		switch (map.id) {
			// in this case we check object can stoop simon moving like ground, block, wall, etc.. 
			// then update the velocity and position
		case OBBoundary:
			checkCollisionWithBoundary(dt, map.objs);
			break;
		case OBItem: checkCollisionWithItems(dt, map.objs);
			break;
		case OBCanHitObjs: updateWhip(dt, map.objs);
			break;
		case OBEnemy:
			updateWhip(dt, map.objs);
		case OBBullet:
			// check can hurt when collide
			// bullet and enemy can hurt simon=> don't have break in enemy case
			checkCollisionWithEnemy(dt, map.objs);
			break;
		default:;
		}
	}
}


CollisionResult Simon::checkCollisionWithBoundary(DWORD dt, vector<LPGAMEOBJECT>* boundaries)
{
	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;
	float minTx;
	float minTy;
	float nx = 0;
	float ny;
	coEvents.clear();
	CollisionResult result = { false, false };
	bool updatedY = false;
	auto updatedX = false;

	calcPotentialCollisions(boundaries, coEvents);

	if (!coEvents.empty()) {
		filterCollision(coEvents, coEventsResult, minTx, minTy, nx, ny);
		auto isCollideDoor = false;

		for (auto& i : coEventsResult) {
			const auto object = (i->obj);
			const auto door = dynamic_cast<Door*>(object);
			{
				const auto boundary = dynamic_cast<Boundary*>(object);
				if (boundary) {
					auto type = boundary->getBoundaryType();
					switch (type) {
					case BoundaryNormal:
						result.x = processCollisionWithBoundaryByX(minTx, nx, boundary);
						break;
					case BoundaryGround:
						result.y = processCollisionWithGround(minTy, ny);
						break;
					case BoundaryTwoSide:
						result.x = processCollisionWithBoundaryByX(minTx, nx, boundary);
						if (ny != 0) {
							blockY(minTy, ny);
							vy = 0;
						}
						setState(idle);
						break;
					case BoundaryBrokenWall:
						result.x = processCollisionWithBoundaryByX(minTx, nx, boundary);
						result.y = processCollisionWithGround(minTy, ny);
						break;
					default:
						break;
					}
				}
			}
		}

		if (isCollideDoor && nx == CDIR_LEFT) {
			blockX(minTx, nx);
			updatedX = true;
		}
	}


	if (!updatedX) {
		if (result.x) {
			auto distance = state == deflect ? 5.0f : 0.1f;
			blockX(minTx, nx, distance);
			vx = 0;
			if (state == deflect) {
				timerDeflect->stop();
				doActionAfterDeflect();
			}
		}
		else x += dx;
	}
	if (!updatedY) {
		if (result.y && ny == CDIR_BOTTOM) {
			vx = 0;
			vy = 0;
			if (state == jumping && !timerSitWhenCollideGround->isRunning() || isHittingWhenJumping) {
				stand();
				isHittingWhenJumping = false;
			}
			isInGround = true;
		}
		else y += dy;
	}



	for (auto& coEvent : coEvents) delete coEvent;
	return result;
}

void Simon::checkCollisionWithItems(DWORD dt, vector<GameObject*>* items) {
	// no collison => check case inside
	for (auto& coObject : *items) {
		const auto item = dynamic_cast<Item*>(coObject);
		if (item) {
			auto coBox = item->getBoundingBox();
			if (isColliding(getBoundingBox(), coBox))
				processCollisionWithItem(item);
		}
	}
}

void Simon::checkCollisionWithEnemy(DWORD dt, vector<GameObject*>* objs) {
	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;
	coEvents.clear();

	calcPotentialCollisions(objs, coEvents);
	calcPotentialCollisionsAABB(objs, coEvents);

	// no collison => check case inside
	if (!coEvents.empty()) {
		float minTx;
		float minTy;
		float nx = 0;
		float ny;
		filterCollision(coEvents, coEventsResult, minTx, minTy, nx, ny);

		for (auto& i : coEventsResult) {
			const auto enemy = dynamic_cast<Enemy*>(i->obj);
			if (enemy && enemy->getState() != death && getHurt(nx, ny, enemy->getDmg())) {
				resetState();
				if (enemy->getEnemyType() == EnemBat) enemy->getHurt(1, 1, 1);
				continue;
			}

			const auto bullet = dynamic_cast<Bullet*>(i->obj);
			if (bullet && bullet->getState() != death && getHurt(nx, ny, bullet->getDmg())) resetState();
		}
	}

	for (auto& coEvent : coEvents) delete coEvent;
}

void Simon::processCollisionWithItem(Item* item) {
	const auto itemType = item->getItemType();
	const auto itemHeart = dynamic_cast<ItemHeart*>(item);

	auto moneyBag = dynamic_cast<ItemMoneyBag*>(item);
	if (moneyBag) {
		StageManager::getInstance()->addScore(moneyBag->getScore());
	}
	else if (itemHeart) {
		energy += itemHeart->getEnergy();
	}
	else {
		switch (itemType) {
		case itemWhip:
			powerUpWhip();
			break;
		case itemPorkChop:
			addHP(6);
			break;
		case itemDoubleShot:
			timerCollectExtraShot->start();
			setCanShotTimes(2);
			break;
		case itemTripleShot:
			timerCollectExtraShot->start();
			setCanShotTimes(3);
			break;
		case itemGoldPotion:
			doUntouchable(SIM_ITEM_GOLD_POTION_DURATION);
			break;
		case itemHolyCross:
			StageManager::getInstance()->doThunderEffect();
			StageManager::getInstance()->clearMapByItem();
			break;
		default:
			subWeaponType = itemType;
			break;
		}
	}

	item->setCollected(true);
}

/*----------------- actions  -----------------*/
void Simon::move(int side) {
	setFaceSide(side);
	setState(walking);
	isStopAllAction = false;
	vx = SIMON_VX * faceSide;
	gravity = SIMON_GRAVITY;
}

void Simon::jump() {
	setState(jumping);
	vy = -SIMON_VJUMP;
	isInGround = false;
	gravity = initGravity;
}

void Simon::sit() {
	if (!isInGround) return;
	setState(sitting);
	vx = 0;
	vy = 0;
	gravity = initGravity;
}

void Simon::stand() {
	resetState();
	vx = 0;
	setState(idle);
}

void Simon::stopMoveWhenHitting() {
	if (state == jumping) return;
	vx = 0;
}

void Simon::hit(int type) {
	if (isDeflecting()) return;
	stopMoveWhenHitting();
	isHitting = true;
	whip->refreshAnim();
	whip->refreshState();
	setState(type);
}

void Simon::doThrow(int type) {
	if (!canThrow()) {
		hit(type - 3);
		return;
	}
	if (subWeaponType == itemStopWatch) {
		StageManager::getInstance()->stopEnemyForABit();
		loseEnergy(5);
		return;
	}
	stopMoveWhenHitting();
	isThrowing = true;
	staringStatus = none;
	setState(type);
}

void Simon::throwSubWeapon() {
	if (canThrow()) generateSubWeapon();
}

void Simon::generateSubWeapon() {
	loseEnergy();

	subWeapon = subWeaponFactory->getSubWeapon(subWeaponType, getFaceSide());
	const auto width = getBoundingBox().r - getBoundingBox().l;
	const auto subX = getFaceSide() == SideLeft ? x - width + 10 : x + width;
	const auto subY = state == throwingWhenSitting ? y + 15 : y;

	subWeapon->setInitPos({ subX, subY });
	subWeapon->setPos(subX, subY);
	subWeapon->setEnable();
	StageManager::getInstance()->addSubWeapon(subWeapon);
	timerThrowing->start();
}

/*----------------- keyboard handle  -----------------*/

void Simon::handleOnKeyRelease(int KeyCode) {
	switch (KeyCode) {
	case DIK_DOWN: {
		isReleaseSitButton = true;
		if (isInGround && !isHitting && !isThrowing && state != climbing)
			stand();
	}
				 break;
	case DIK_UP:
		isReleaseThrowButton = true;
		break;
	default:;
	}
}

void Simon::handleOnKeyPress(BYTE* states) {
	auto game = Game::getInstance();

	if (shouldKeyboardDisable()) return;
	if (state == sitting) return;
	if (state == jumping) return;

	if (game->isKeyDown(DIK_RIGHT)) {
		move(SideRight);
	}
	else if (game->isKeyDown(DIK_LEFT)) {
		move(SideLeft);
	}
	else if (game->isKeyDown(DIK_DOWN)) {
		isReleaseSitButton = false;
		sit();
	}
	else {
		stand();
	}
}

void Simon::handleOnKeyDown(int keyCode) {
	if (shouldKeyboardDisable()) return;

	switch (keyCode) {
	case DIK_SPACE:
		if (previousState != jumping && isInGround && !isHitting && state != climbing)
			jump();
		break;
	case DIK_LCONTROL: {
		if (state == jumping) isHittingWhenJumping = true;
		auto hittingType = getHittingInfo();
		if (isReleaseThrowButton) hit(hittingType);
		else doThrow(hittingType + 3);
		break;
	}
	case DIK_A: {
		if (state == jumping) isHittingWhenJumping = true;
		auto hittingType = getHittingInfo();
		this->doThrow(hittingType + 3);
		break;
	}

	default:;
	}
}

bool Simon::shouldKeyboardDisable() {
	return forceDead || isHitting || isThrowing || isPowering() || isAutoWalking() ||
		isDeflecting() || isDying() || forceDisable;
}

/*----------------- utilities  -----------------*/

void Simon::updateAnimId() {
	if (isAutoWalking()) {
		setAnimId(ANIM_WALK);
		GameObject::updateAnimId();
		return;
	}
	if (isDying()) {
		setAnimId(ANIM_DEATH);
		return;
	}
	if (isChangingStage()) {
		setAnimId(changeStateAnim);
		return;
	}
	int frame;
	switch (state) {
	case climbing:
		setAnimId(climbDirection == ClimbUp ? ANIM_UP_STAIR : ANIM_DOWN_STAIR);
		break;
	case walking: setAnimId(ANIM_WALK);
		break;
	case sitting:
		if (isReleaseSitButton) stand();
		else setAnimId(ANIM_SIT);
		break;
	case jumping:
		setAnimId((vy > SIM_VY_READY_TO_LAND) ? ANIM_IDLE : ANIM_SIT);
		break;
	case hitting:
		setAnimId(ANIM_HITTING);
		if (animations[animId]->isDone())
			refreshHitAnim();
		break;
	case hittingWhenSitting:
		setAnimId(ANIM_HITTING_WHEN_SIT);
		if (animations[animId]->isDone())
			refreshHitAnim(sitting, ANIM_SIT);
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
	case idleBack:
		setAnimId(ANIM_IDLE_BACK);
		vx = 0;
		vy = 0;
		break;
	default:
		setAnimId(ANIM_IDLE);
	}
	GameObject::updateAnimId();
}

void Simon::refreshHitAnim(int stateAfterHit, int animAfterHit) {
	whip->refreshAnim();
	isHitting = false;
	isThrowing = false;
	animations[animId]->refresh();
	if (stateAfterHit != -1)setState(stateAfterHit);
	if (animAfterHit != -1)setAnimId(animAfterHit);
}

void Simon::resetState() {
	refreshHitAnim(-1, -1);
	whip->refreshState();
	animations[ANIM_HITTING]->refresh();
	animations[ANIM_HITTING_WHEN_SIT]->refresh();
	timerSitWhenCollideGround->stop();
	animations[animId]->refresh();
}

void Simon::reset() {
	resetState();
	staringStatus = none;
	stateAfterAutoWalk = -1;
	nxAfterAutoWalk = -1;
	changeStateDistanceRemain = { -1, -1 };
	gravity = initGravity;
	timerPowering->stop();
	timerDeflect->stop();
	timerUntouchable->stop();
	timerDeath->stop();
	autoWalkDistance = 0;
	climbDirection = 0;
	stairDxRemain = 0;
	stairDyRemain = 0;
	canDeflect = true;
	r = g = b = 255;
	setSubWeapon(-1);
	setLvWhip(1);
	setCanShotTimes(1);
	setSpeed(0, 0);
}

bool Simon::getHurt(int nx, int ny, int hpLose) {
	if (isUntouching() || isDeflecting()) return false;
	if (GameObject::getHurt(nx, ny, hpLose)) {
		resetState();
		return true;
	}

	return false;
}

void Simon::upgradeWhipLv(bool up) const {
	whip->upgradeWhipLv(up);
}

void Simon::powerUpWhip(bool upgrade) {
	if (whip) whip->upgradeWhipLv(upgrade);
	if (isPowering() && isHitting) return;
	timerPowering->start();
	vx = 0;
}

bool Simon::canThrow() {
	return energy > 0 && StageManager::getInstance()->getCurrentSubWeaponsAmount() < canShotTimes && isHaveSubWeapon() && timerThrowing->isTimeUp();
}

int Simon::getHittingInfo() const {
	const auto isJumpingHit = state == jumping;
	auto hittingType = hitting;
	if (!isReleaseSitButton && !isJumpingHit) hittingType = hittingWhenSitting;
	else if (state == climbing) hittingType = hittingWhenClimbing;
	return hittingType;
}

bool Simon::canAutoWalkWithDistance() const {
	return autoWalkDistance > 0;
}

bool Simon::canAutoClimb() const {
	return stairDxRemain > 0 || stairDyRemain > 0;
}

bool Simon::isAutoWalking() const {
	return canAutoWalkWithDistance();
}

bool Simon::addLife(int val) {
	life += val;
	if (life < 0) {
		life = 0;
		return false;
	}
	return true;
}

bool Simon::addHP(int val) {
	if (val > 1) {
		hpWillAdd = val - 1;
		hp += 1;
		if (hp > SIM_MAX_HP) hp = SIM_MAX_HP;
		else timerAddHp->startDeep();
		return true;
	}
	hp += val;
	if (hp <= 0) {
		hp = 0;
		return false;
	}

	if (hp > SIM_MAX_HP) hp = SIM_MAX_HP;
	return true;
}

void Simon::addEnergy(int val/*=1*/) {
	energy += val;
	energy = energy > SIM_MAX_ENERGY ? SIM_MAX_ENERGY : energy;
}

void Simon::setLvWhip(int lv)
{
	whip->setLv(lv);
}

Simon::~Simon()
= default;