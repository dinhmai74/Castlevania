#pragma once
#include "GameObject.h"
#include "Whip.h"
#include "Constants.h"
#include "SetTimeOut.h"
#include <conio.h>
#include "Item.h"
#include "SubWeaponFactory.h"
#include "Boundary.h"
#include "Timer.h"
#include "Stair.h"
#include "ObjectChangeStage.h"
#include "Door.h"

class Simon :
	public GameObject {
public:
	Simon();
	void init();
	~Simon();

	void handleOnKeyPress(BYTE* states);
	bool shouldKeyboardDisable();
	void handleOnKeyDown(int KeyCode);


	Box getBoundingBox() override;
	void handleOnKeyRelease(int KeyCode);

	void resetState();

	// Inherited via GameObject
	void setState(int state);

	void render() override;

	void didSimonRender();

	void renderWhip();

	bool checkClimbingState();
	virtual void updateAnimId();

	void refreshHitAnim(int stateAfterHit = idle, int animAfterHit = ANIM_IDLE);

	void initAnim() override;

	bool shouldUpdate(DWORD dt);
	void willUpdate();
	void update(DWORD dt, const vector<MapGameObjects>& maps);
	void didUpdate();


	bool updateLife(int val);
	bool updateHP(int val);
	void updateEnergy(int val = 1);
	void doAutoWalk(DWORD dt = 1000, float vx = SIM_AUTO_WALK_VX);
	void doAutoWalkWithDistance(float distance, float vx = SIM_AUTO_WALK_DISTANCE_VX);

	/*----------------- get set  -----------------*/
	SubWeapon* getSubWeapon() const { return subWeapon; }
	int  getSubWeaponType() const { return subWeaponType; }
	void powerUpWhip(bool upgrade = true);
	Whip* getWhip() { return whip; }
	int getEnergy() { return energy; };
	int Life() const { return life; }
	void setLife(int val) { life = val; }
	void reset();
	void setEnergy(int val) { energy = val; };
	bool getHurt(int nx, int ny, int hpLose) override;
	float getStairDxRemain() const { return stairDxRemain; }
	void setStairDxRemain(float val) { stairDxRemain = val; }
	float getStairDyRemain() const { return stairDyRemain; }
	void setStairDyRemain(float val) { stairDyRemain = val; }
	int getClimbDirection() const { return climbDirection; }
	void setClimbDirection(int val) { climbDirection = val; }

	float getAutoWalkDistance() const { return autoWalkDistance; }
	void setAutoWalkDistance(float val) { autoWalkDistance = val; }
	void setSubWeapon(int val) { subWeaponType = val; }
	bool isWalkingOutDoor() { return collidedDoor != nullptr; }
private:
	bool isHitting{};
	bool isThrowing{};
	bool isCollectingWhip{};
	Timer* timerPowering = new Timer(SIM_POWERING_DURATION);
	Timer* timerThrowing = new Timer(SIM_DELTA_TRHOWING_TIME);
	Timer* timerAutoWalk = new Timer(SIM_AUTO_WALK_DURATION);
	float autoWalkDistance;
	bool isReleaseSitButton{};
	Whip* whip{};
	SubWeapon* subWeapon{};
	bool startedChangeStage{};
	int subWeaponType{};
	bool isInGround{};
	int energy{};
	int life{};
	int climbDirection{};
	int stageIdWillChangeTo{};
	bool forceDead{};
	int staringStatus{};
	float stairDxRemain{};
	float stairDyRemain{};
	Stair* collidedStair{};
	vector<GameObject*>* listStairs{};
	int goThroughDoorStatus;
	D3DXVECTOR2 changeStateVelocity;
	VectorInt changeStateDirection;
	D3DXVECTOR2 changeStateDistanceRemain;

	bool isPowering() { return isTimerRunning(timerPowering); };
	bool isChangingStage() { return changeStateDistanceRemain.x >= 0 && changeStateDistanceRemain.y >= 0; };
	bool isCollidingWithStair();
	void setClimbStairInfo(int direction);
	bool forceStopClimb(int direction);
	void removeAutoClimbDistance();
	int getHittingInfo();


	/*----------------- simon actions -----------------*/
	void move(int side);
	void jump();
	void sit();
	void stand();
	bool climbStair(int direction);
	void stopMoveWhenHitting();
	void hit(int type);
	bool canThrow();
	void doThrow(int type);
	void throwSubWeapon();
	void loseEnergy() { energy--; energy = energy < 0 ? 0 : energy; };
	void addEnergy() { energy++; energy = energy > SIM_MAX_ENERGY ? SIM_MAX_ENERGY : energy; };
	void generateSubWeapon();
	bool isHaveSubWeapon() const {
		return subWeaponType != -1;
	};

	bool canAutoWalkWithDistance();

	/*----------------- special effect  -----------------*/
	void checkCollisionWithObChangeStage(DWORD dt, vector<GameObject*>* objs);
	void processDeathEffect();
	void updateAutoWalk();
	void moveCam(float distance = 235);

	/*----------------- check collision -----------------*/
	CollisionResult checkCollisionWithBoundary(DWORD dt, vector<LPGAMEOBJECT>* boundaries);
	void checkCollisionWithItems(DWORD dt, vector<GameObject*>* items);
	void checkCollisionWithEnemy(DWORD dt, vector<GameObject*>* objs);
	void upgradeWhipLv(bool up = true) const;
	void updateWhip(DWORD dt, vector<GameObject*>* objs);
	void updateRGB();
	void checkCollision(DWORD dt, const vector<MapGameObjects>& map);

	void processCollisionWithItem(Item* item);
	void updateChangingStageEffect();
	void doChangeStageEffect(ObjectChangeStage* obj, DWORD changingDuration = SIM_CHANGING_STAGE_DURATION);
	void checkCollisionWithStair(vector<GameObject*>* objs);

	bool canAutoClimb();
	void doAutoClimb();
	bool isAutoWalking();
	void checkIfFalling();
	float vxAutoWalk;
	void updateCameraWhenGoThroughDoor();
	bool processCollisionWithDoor(float minTx, float nx, Door* door);
	Door* collidedDoor;
	bool forceDisable;
	void checkOutOfBound();
	void checkCollisionWithForceIdleSim(DWORD dt, vector<GameObject*>* objs);
	bool isReleaseThrowButton;
	int changeStateAnim;
	std::wstring stageMapObjNameWillChangeto;
	bool movingCam;
};
