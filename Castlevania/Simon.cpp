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
	climbDirection = ClimbNull;
	staringStatus = none;
	goThroughDoorStatus = nope;
	changeStateDistanceRemain = { -1, -1 };
	whip->setPos(x, y);

	isInGround = false;
	isReleaseSitButton = true;
	isReleaseThrowButton = true;
	Simon::initAnim();
	setAnimId(ANIM_IDLE);
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
}

void Simon::render() {
	if (state == StateNone) return;
	renderWhip();
	auto isPauseAnim = isStopAllAction || (staringStatus == pause && !isHitting);
	animations[animId]->render(faceSide, x, y, alpha, r, g, b, isPauseAnim,		currentFrame);
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

bool Simon::checkClimbingState() {
	if (state == climbing && staringStatus == pause && !canAutoClimb()) {
		currentFrame = 0;
		return true;
	}

	return false;
}

/*----------------- update  -----------------*/

bool Simon::shouldUpdate(DWORD dt) {
	this->dt = dt;
	canDeflect = state != climbing && staringStatus != ready;
	updateRGB();
	return !forceDead && !isStopAllAction;
}

void Simon::willUpdate() {
	updateAutoWalk();
	doAutoClimb();
	checkIfFalling();
	processDeflectEffect();
	updateChangingStageEffect();
	updateCameraWhenGoThroughDoor();
	processDeathEffect();
	if (state != climbing && state != hittingWhenClimbing && state != throwingWhenClimbing) updateGravity(dt);
}

void Simon::update(DWORD dt, const vector<MapGameObjects>& maps) {
	if (!shouldUpdate(dt)) return;

	willUpdate();
	GameObject::update(dt);

	checkCollision(dt, maps);
	checkClimbingState();
	updateAnimId();
	processSitWhenCollideGroundAnim();

	didUpdate();
}

void Simon::didUpdate() {
	if (movingCam) {
		if (!Camera::getInstance()->isMoving()) {
			StageManager::getInstance()->removeAllObjOutOfBound();
			movingCam = false;
		}
	}
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
}

void Simon::updateCameraWhenGoThroughDoor() {
	if (!collidedDoor) return;
	auto cam = Camera::getInstance();
	switch (goThroughDoorStatus) {
	case ThroughDoorStarted:
		if (!cam->isMoving()) {
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
		StageManager::getInstance()->setCheckPoint(collidedDoor->getNewCheckPoint());
		moveCam(collidedDoor->getMoveCamDistance());
		movingCam = true;
		collidedDoor = nullptr;
		break;
	default:;
	}
}

void Simon::updateRGB() {
	auto const stageManager = StageManager::getInstance();
	if (timerPowering->isRunning()) {
		r = g + 80;
		g = 100 + rand() % 150;
		b = g + 35;
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
	if (timerAutoWalk->isRunning())
		vx = vxAutoWalk * float(faceSide);
	else if (canAutoWalkWithDistance()) {
		vx = vxAutoWalk * float(faceSide);
		autoWalkDistance -= vxAutoWalk * dt;
	}
	else if (staringStatus == ready) setClimbStairInfo(climbDirection);
}

void Simon::updateChangingStageEffect() {
	if (isChangingStage()) {
		// base on distance, in update will auto make simon walk or climb
		vx = changeStateDirection.x * changeStateVelocity.x;
		vy = changeStateDirection.y * changeStateVelocity.y;
		changeStateDistanceRemain.x -= vx * dt;
		changeStateDistanceRemain.y -= vy * dt;
		startedChangeStage = true;
	}
	else {
		if (startedChangeStage) {
			StageManager::getInstance()->nextStage(stageIdWillChangeTo, stageMapObjNameWillChangeto);
			startedChangeStage = false;
		}
	}
}

void Simon::updateWhip(DWORD dt, vector<GameObject*>* objs) {
	whip->update(dt, x, y, objs, state, climbDirection);
}

void Simon::doChangeStageEffect(ObjectChangeStage* obj, DWORD duration) {
	if (isChangingStage()) return;

	auto destinationPoint = obj->getChangeStateDestinationPoint();
	if (destinationPoint.x == -1 && destinationPoint.y == -1) {
		StageManager::getInstance()->nextStage(stageIdWillChangeTo, obj->getNextStageMapObjName());
		return;
	}
	changeStateAnim = obj->getChangeStateAnimId();
	stageMapObjNameWillChangeto = obj->getNextStageMapObjName();
	auto xDistance = destinationPoint.x - x;
	// > 0 => need to go right
	faceSide = xDistance > 0 ? 1 : -1;
	auto yDistance = destinationPoint.y - y;
	auto yDirection = yDistance > 0 ? 1 : -1;

	changeStateVelocity = obj->getChangeStateVelocity();
	changeStateDirection = { faceSide, yDirection };
	changeStateDistanceRemain = { fabs(xDistance), fabs(xDistance) };
}

void Simon::doAutoClimb() {
	if (!canAutoClimb()) {
		return;
	}
	if (state == climbing) {
		const auto climbSpeed = SIM_CLIMB_VELOCITY;
		vx = climbSpeed * faceSide;
		vy = static_cast<float>(climbDirection) * -climbSpeed;
		setStairDxRemain(getStairDxRemain() - float(dt) * climbSpeed);
		setStairDyRemain(getStairDyRemain() - float(dt) * climbSpeed);
	}
}

void Simon::checkIfFalling() {
	if (state == death) return;
	const auto isFailing = (vy > 0.2 && state != jumping && previousState != jumping && state != hitting && previousState != hitting) || vy > 0.47;
	if (isFailing) doFall();
	else isStopAllAction = false;
}

void Simon::doFall() {
	setState(idle);
	refreshHitAnim(-1, -1);
	gravity = initGravity * 15;
	isFalling = true;
	isInGround = false;
	isStopAllAction = true;
}

void Simon::doAutoWalk(DWORD dt, float vx) {
	if (timerAutoWalk->isRunning()) return;
	timerAutoWalk->setLimitedTime(dt);
	vxAutoWalk = vx;
	timerAutoWalk->start();
}

void Simon::doAutoWalkWithDistance(float distance, float vx) {
	if (distance < 0) faceSide = SideLeft;
	else faceSide = SideRight;
	vxAutoWalk = vx;
	autoWalkDistance = fabs(distance);
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
	for (auto map : maps) {
		switch (map.id) {
		case OBBoundary:
			if (state != climbing) checkCollisionWithBoundary(dt, map.objs);
			else updatePosWhenNotCollide();
			break;
		case OBItem: checkCollisionWithItems(dt, map.objs);
			break;
		case OBCanHitObjs: updateWhip(dt, map.objs);
			break;
		case OBChangeStage:
			checkCollisionWithObChangeStage(dt, map.objs);
			break;
		case OBEnemy:
			updateWhip(dt, map.objs);
		case OBBullet:
			checkCollisionWithEnemy(dt, map.objs);
			break;
		case OBStair:
			listStairs = map.objs;
			checkCollisionWithStair(listStairs);
			break;
		case OBForceIdleSim:
			checkCollisionWithForceIdleSim(dt, map.objs);
			break;
		case OBWater:
			checkCollisionWithWater(map.objs);
			break;
		default:;
		}
	}
}

void Simon::checkCollisionWithObChangeStage(DWORD dt, vector<GameObject*>* objs) {
	if (state == death) return;
	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;
	coEvents.clear();

	calcPotentialCollisions(objs, coEvents);
	if (!coEvents.empty()) {
		float minTx, minTy, nx, ny;
		filterCollision(coEvents, coEventsResult, minTx, minTy, nx, ny);
		for (auto& i : coEventsResult) {
			const auto obj = dynamic_cast<ObjectChangeStage*>(i->obj);
			if (obj) {
				const auto side = obj->getSimonDirectCollide();
				stageIdWillChangeTo = obj->getNextStageId();
				stageMapObjNameWillChangeto = obj->getNextStageMapObjName();
				if (side.y != 0) {
					if (ny == side.y) doChangeStageEffect(obj);
				}
				else doChangeStageEffect(obj);
			}
		}
	}
}

void Simon::checkCollisionWithStair(vector<GameObject*>* objs) {
	collidedStair = nullptr;
	for (auto obj : *objs) {
		const auto stair = dynamic_cast<Stair*>(obj);
		auto box = getBoundingBox();
		box.t = box.b - 5;
		if (isColliding(box, obj->getBoundingBox()) && stair)
			collidedStair = stair;
	}
}

CollisionResult Simon::checkCollisionWithBoundary(DWORD dt, vector<LPGAMEOBJECT>* boundaries) {
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
			if (door)
				isCollideDoor = processCollisionWithDoor(minTx, nx, door);
			else {
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
			if (isFalling) timerSitWhenCollideGround->startDeep();
			else if (state == jumping && !timerSitWhenCollideGround->isRunning()) stand();
			isInGround = true;
			isFalling = false;
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
			if (enemy && getHurt(nx, ny, enemy->getDmg())) {
				resetState();
				if (enemy->getEnemyType() == EnemBat) enemy->getHurt(EnemyFactory::getInstance()->getHp(EnemBat));
				continue;
			}

			const auto bullet = dynamic_cast<Bullet*>(i->obj);
			if (bullet && getHurt(nx, ny, bullet->getDmg())) resetState();
		}
	}

	for (auto& coEvent : coEvents) delete coEvent;
}

bool Simon::processCollisionWithDoor(float minTx, float nx, Door* door) {
	auto result = false;
	if ((nx == door->getFaceSide()) && !door->isOpenning()) {
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
		case itemHolyCross:
			StageManager::getInstance()->clearMapByItem();
			break;
		default:
			subWeaponType = itemType;
			break;
		}
	}

	item->setActive(false);
}

void Simon::checkCollisionWithForceIdleSim(DWORD dt, vector<GameObject*>* objs) {
	for (auto obj : *objs) {
		auto force = dynamic_cast<ForceIdleSim*>(obj);
		if (force && isColliding(getBoundingBox(), force->getBoundingBox())) {
			auto res = forceStopClimb(force->getDirection());
			if (res) {
				y = force->getNextY();
				staringStatus = none;
				isInGround = true;
			}
		}
	}
}

/*----------------- actions  -----------------*/
bool Simon::climbStair(int direction) {
	if (!isCollidingWithStair()) {
		vx = 0;
		vy = 0;
		staringStatus = none;
		return false;
	}
	if (staringStatus == ready || getStairDxRemain() > 0 || getStairDyRemain() > 0) return false;

	if (collidedStair->getStairType() == StairStartUp && direction != ClimbUp) return false;
	if (collidedStair->getStairType() == StairStartDown && direction != ClimbDown) return false;

	if (state != climbing) {
		const auto collidePos = collidedStair->getPos();
		const auto finalStandPos = collidePos.x - (getBoundingBox().l - x) - 5 * collidedStair->getFaceSide();
		doAutoWalkWithDistance(finalStandPos - x);
		staringStatus = ready;
	}
	else {
		setClimbStairInfo(direction);
	}
	climbDirection = direction;

	return true;
}

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
	StageManager::getInstance()->add(subWeapon);
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
		if (state == climbing) {
			if (collidedStair)
				climbStair(ClimbUp * collidedStair->getFaceSide());
		}
		else move(SideRight);
	}
	else if (game->isKeyDown(DIK_LEFT)) {
		if (state == climbing) {
			if (collidedStair)
				climbStair(ClimbDown * collidedStair->getFaceSide());
		}
		else move(SideLeft);
	}
	else if (game->isKeyDown(DIK_DOWN)) {
		isReleaseSitButton = false;
		auto result = false;
		if (collidedStair && collidedStair->getStairType() == StairStartDown && state != jumping)
			result = climbStair(ClimbDown);
		else if (state != climbing) {
			sit();
			result = true;
		}
		else result = climbStair(ClimbDown);
		if (!result) stand();
	}
	else if (game->isKeyDown(DIK_UP)) {
		if (state != jumping) {
			auto result = climbStair(ClimbUp);
			if (!result) stand();
		}
		isReleaseThrowButton = false;
	}
	else if (state == climbing) {
		staringStatus = pause;
		vx = 0;
		vy = 0;
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
		auto hittingType = getHittingInfo();
		if (isReleaseThrowButton) hit(hittingType);
		else doThrow(hittingType + 3);
		break;
	}
	case DIK_A: {
		auto hittingType = getHittingInfo();
		this->doThrow(hittingType + 3);
		break;
	}

	default:;
	}
}

bool Simon::shouldKeyboardDisable() {
	return forceDead || isHitting || isThrowing || isPowering() || isAutoWalking() || isChangingStage() ||
		isDeflecting() || isDying() || canAutoClimb() || forceDisable || goThroughDoorStatus != nope || Camera::
		getInstance()->isMoving() || timerSitWhenCollideGround->isRunning();
}

/*----------------- utilities  -----------------*/

void Simon::updateAnimId() {
	if (isAutoWalking() || isDying()) {
		setAnimId(ANIM_WALK);
		GameObject::updateAnimId();
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

	case hittingWhenClimbing:
	{
		auto const anim = climbDirection == ClimbUp ? ANIM_HIT_UP_STAIR : ANIM_HIT_DOWN_STAIR;
		setAnimId(anim);
		if (animations[animId]->isDone())
			refreshHitAnim(climbing, anim);
		break;
	}
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
	case throwingWhenClimbing: {
		auto const anim = climbDirection == ClimbUp ? ANIM_HIT_UP_STAIR : ANIM_HIT_DOWN_STAIR;
		setAnimId(anim);
		frame = animations[animId]->getCurrentFrame();
		if (frame == 2) throwSubWeapon();
		else if (animations[animId]->isDone())
			refreshHitAnim(climbing, anim);
		break;
	}
	default:
		setAnimId(ANIM_IDLE);
	}
	GameObject::updateAnimId();
}

bool Simon::processSitWhenCollideGroundAnim() {
	if (timerSitWhenCollideGround->isRunning()) {
		setAnimId(ANIM_SIT);
		return true;
	}
	if (timerSitWhenCollideGround->isTimeUpAndRunAlr()) {
		stand();
		timerSitWhenCollideGround->stop();
		return true;
	}

	return false;
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
	changeStateDistanceRemain = { -1, -1 };
	gravity = initGravity;
	timerPowering->stop();
	timerDeflect->stop();
	timerThrowing->stop();
	timerUntouchable->stop();
	timerAutoWalk->stop();
	timerDeath->stop();
	autoWalkDistance = 0;
	climbDirection = 0;
	stairDxRemain = 0;
	stairDyRemain = 0;
	canDeflect = true;
	isFalling = false;
	setSpeed(0, 0);
}

bool Simon::getHurt(int nx, int ny, int hpLose) {
	if (isUntouching() || isDeflecting()) return false;
	resetState();
	return GameObject::getHurt(nx, ny, hpLose);
}

void Simon::setDeathByWater() {
	state = StateNone;
	if (isDying()) return;

	timerDeath->start();
	vx = 0;
	gravity = 0;
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

bool Simon::isCollidingWithStair() {
	collidedStair = nullptr;
	checkCollisionWithStair(listStairs);
	return collidedStair != nullptr;
}

bool Simon::canThrow() {
	return energy > 0 && timerThrowing->isTimeUp() && isHaveSubWeapon();
}

void Simon::setClimbStairInfo(int direction) {
	setState(climbing);
	faceSide = collidedStair->getFaceSide() * direction;
	const auto nextPos = collidedStair->getNextPos();
	setStairDxRemain(nextPos.x);
	setStairDyRemain(nextPos.y);
	gravity = 0;
	staringStatus = onGoing;
}

bool Simon::forceStopClimb(int direction) {
	if (state != climbing || climbDirection != direction) return false;
	removeAllVelocity();
	removeAutoClimbDistance();
	collidedStair = nullptr;
	stand();
	return true;
}

void Simon::removeAutoClimbDistance() {
	setStairDxRemain(-1);
	setStairDyRemain(-1);
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
	return stairDxRemain > 0 && stairDyRemain > 0;
}

bool Simon::isAutoWalking() const {
	return timerAutoWalk->isRunning() || canAutoWalkWithDistance();
}

void Simon::moveCam(float distance) const {
	if (!Camera::getInstance()->isMoving()) Camera::getInstance()->move(distance);
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

Simon::~Simon()
= default;
