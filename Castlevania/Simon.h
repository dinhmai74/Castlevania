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
	void handleOnKeyRelease(int KeyCode);

	void resetState();

	void render() override;
	void didSimonRender();
	void renderWhip();

	virtual void updateAnimId();


	void refreshHitAnim(int stateAfterHit = idle, int animAfterHit = ANIM_IDLE);

	void initAnim() override;

	bool shouldUpdate(DWORD dt);
	void willUpdate();
	void autoUpdateHp();
	void update(DWORD dt, const vector<MapGameObjects>& maps);
	void didUpdate();


	bool addLife(int val);
	bool addHP(int val);
	void addEnergy(int val = 1);
	void doAutoWalkWithDistance(float distance, float vx = SIM_AUTO_WALK_DISTANCE_VX);
	void doAutoWalkWithDistance(float distance, float vx,int newState, int newFaceSide);

	/*----------------- get set  -----------------*/
	void setLvWhip(int lv);
	SubWeapon* getSubWeapon() const { return subWeapon; }
	int  getSubWeaponType() const { return subWeaponType; }
	void powerUpWhip(bool upgrade = true);
	Whip* getWhip() { return whip; }
	int getEnergy() { return energy; }
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
	void loseEnergy(int val=1) { energy-=val; energy = energy < 0 ? 0 : energy; };
	void addEnergy() { energy++; energy = energy > SIM_MAX_ENERGY ? SIM_MAX_ENERGY : energy; };


	void setCanShotTimes(int val) { canShotTimes = val; }
	int getCanShotTimes() const { return canShotTimes; }
	bool isTimerCollectExtraShotRunning() { return timerCollectExtraShot->isRunning(); }
private:
	bool isHitting{};
	bool isHittingWhenJumping;
	bool isThrowing{};
	bool isCollectingWhip{};
	Timer* timerPowering = new Timer(SIM_POWERING_DURATION);
	Timer* timerSitWhenCollideGround= new Timer(SIM_SIT_WHEN_LANDING);
	Timer* timerThrowing = new Timer(SIM_DELTA_TRHOWING_TIME);
	Timer* timerCollectExtraShot = new Timer(SIM_EXTRA_SHOT_DURATION);
	Timer* timerAddHp = new Timer(100);
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
	int canShotTimes;
	int hpWillAdd;

	bool isPowering() { return isTimerRunning(timerPowering); };
	bool isChangingStage() { return changeStateDistanceRemain.x >= 0 && changeStateDistanceRemain.y >= 0; };
	int getHittingInfo() const;


	/*----------------- simon actions -----------------*/
	void move(int side);
	void jump();
	void sit();
	void stand();
	void stopMoveWhenHitting();
	void hit(int type);
	bool canThrow();
	void doThrow(int type);
	void throwSubWeapon();
	void generateSubWeapon();
	bool isHaveSubWeapon() const {
		return subWeaponType != -1;
	};

	bool canAutoWalkWithDistance() const;

	/*----------------- special effect  -----------------*/
	void processDeathEffect();
	void updateAutoWalk();

	/*----------------- check collision -----------------*/
	CollisionResult checkCollisionWithBoundary(DWORD dt, vector<LPGAMEOBJECT>* boundaries);
	void checkCollisionWithItems(DWORD dt, vector<GameObject*>* items);
	void checkCollisionWithEnemy(DWORD dt, vector<GameObject*>* objs);
	void upgradeWhipLv(bool up = true) const;
	void updateWhip(DWORD dt, vector<GameObject*>* objs);
	void updateRGB();
	void checkCollision(DWORD dt, const vector<MapGameObjects>& map);

	void processCollisionWithItem(Item* item);

	bool canAutoClimb() const;
	bool isAutoWalking() const;
	float vxAutoWalk;
	bool forceDisable;
	void checkOutOfBound();
	bool isReleaseThrowButton;
	bool movingCam;
	int nxAfterAutoWalk;
	int stateAfterAutoWalk;
	int changeStateAnim;
};
