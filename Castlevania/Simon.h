#pragma once
#include "GameObject.h"
#include "Whip.h"
#include "Constants.h"
#include "SetTimeOut.h"
#include <conio.h>
#include "Item.h"
#include "SubWeaponFactory.h"
#include "SubWeapon.h"
#include "Boundary.h"
#include "Timer.h"
#include "Stair.h"

class Simon :
	public GameObject
{
public:
	Simon();
	void init();
	~Simon();

	void handleOnKeyPress(BYTE* states);
	bool isDoingImportantAnim();
	void handleOnKeyDown(int KeyCode);
	Box getBoundingBox() override;
	void handleOnKeyRelease(int KeyCode);

	void resetState();

	// Inherited via GameObject
	void setState(int state);

	void render() override;

	void renderWhip();

	bool forceRenderStaringAnimStand();
	virtual void updateAnimId();

	void refreshHitAnim(int stateAfterHit = idle, int animAfterHit= ANIM_IDLE)
	{
		whip->refreshAnim();
		isHitting = false;
		isThrowing = false;
		animations[animId]->refresh();
		setState(stateAfterHit);
		setAnimId(animAfterHit);
	}

	void initAnim() override;
	void update(DWORD dt, const vector<MapGameObjects>& maps);

	bool updateLife(int val);
	bool updateHP(int val);
	void doAutoWalk(DWORD dt=1000, float vx= SIM_AUTO_WALK_VX);
	void doAutoWalkWithDistance(float distance, float vx= SIM_AUTO_WALK_DISTANCE_VX);

	/*----------------- get set  -----------------*/
	SubWeapon* getSubWeapon() const { return subWeapon; }
	int  getSubWeaponType() const { return subWeaponType; }
	void powerUpWhip(bool upgrade = true);
	Whip* getWhip() { return whip; }
	int getHp() { return hp; };
	int getEnergy() { return energy; };
	int Life() const { return life; }
	void setLife(int val) { life = val; }
	void reset();
	void setHp(int val);
	void setEnergy(int val);
	void setForceDead(bool val=true);
	void setSubWeapon(int type)
	{
		subWeaponType = type;
	};

	void getHurt(int nx, int ny, int hpLose) override;

private:
	bool isHitting{};
	bool isThrowing{};
	bool isCollectingWhip{};
	Timer* timerPowering = new Timer(SIM_POWERING_DURATION);
	Timer* timerThrowing = new Timer(SIM_DELTA_TRHOWING_TIME);
	Timer* timerAutoWalk = new Timer(SIM_AUTO_WALK_DURATION);
	Timer* timerChangeStage = new Timer(SIM_CHANGING_STAGE_DURATION);
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
	int stageWillChangeTo{};
	bool forceDead{};
	int staringStatus{};
	float stairDxRemain{};
	float stairDyRemain{};
	Stair* collidedStair{};

	bool isPowering() { return isTimerRunning(timerPowering); };
	bool isChangingStage() { return isTimerRunning(timerChangeStage); };
	bool isCollidingWithStair();
	void standAfterClimbStair();
	void setClimbStairInfo(int direction);

	/*----------------- simon actions -----------------*/
	void move(int side);
	void jump();
	void sit();
	void stand();
	void standUp();
	bool climbStair(int direction);
	void downStair();
	void stopMoveWhenHitting();
	void hit(int type);
	bool canThrow();
	void doThrow(int type);
	void throwSubWeapon();
	void loseEnergy() { energy--; energy = energy < 0 ? 0 : energy; };
	void addEnergy() { energy++; energy = energy > SIM_MAX_ENERGY ? SIM_MAX_ENERGY : energy; };
	void generateSubWeapon();
	bool isHaveSubWeapon() const
	{
		return subWeaponType != -1;
	};

	bool canAutoWalkWithDistance();

	/*----------------- special effect  -----------------*/
	void checkCollisionWithObChangeStage(DWORD dt, vector<GameObject*>* objs);
	void processDeathEffect();
	void updateAutoWalk(DWORD dt);

	/*----------------- check collision -----------------*/
	void processCollisionWithGround(float minTy, float ny);
	void processCollisionWithBoundaryByX(float minTx, float ny, Boundary* boundary);
	void checkCollisionWithBoundary(DWORD dt, vector<LPGAMEOBJECT>* boundaries);
	void checkCollisionWithItems(DWORD dt, vector<GameObject*>* items);
	void checkCollisionWithEnemy(DWORD dt, vector<GameObject*>* objs);
	void upgradeWhipLv(bool up = true) const;
	void updateWhip(DWORD dt, vector<GameObject*>* objs);
	void updateRGB();
	void checkCollision(DWORD dt, const vector<MapGameObjects>& map);

	void processCollisionWithItem(Item* item);
	void updateChangingStageEffect();
	void doChangeStageEffect();
	void processAnimStaring();
	void checkCollisionWithStair(vector<GameObject*>* objs);

	void removeAutoclimbDistance()
	{
		stairDxRemain = -1;
		stairDyRemain = -1;
	}

	void removeAllVelocity()
	{
		gravity = 0;
		vx = 0;
		vy = 0;
	}

	vector<GameObject*>* listStairs{};
	bool canAutoClimb();
	void doAutoClimb(DWORD dt);
	bool isAutoWalking();
	void checkIfFalling(DWORD dt);
	float vxAutoWalk;
};
