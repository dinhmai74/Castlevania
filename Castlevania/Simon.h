#pragma once
#include "GameObject.h"
#include "Whip.h"
#include "Constants.h"
#include "SetTimeOut.h"
#include <conio.h>
#include "Item.h"
#include "SubWeaponFactory.h"
#include "SubWeapon.h"
#include "Timer.h"

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
	void handleOnKeyRelease(int KeyCode);

	void resetState();

	// Inherited via GameObject
	void setState(int state);

	virtual void render() override;
	void updateAnimId();
	void initAnim() override;
	void update(DWORD dt, const vector<MapGameObjects>& maps);

	SubWeapon* getSubWeapon() const { return subWeapon; }
	int  getSubWeaponType() const { return subWeaponType; }
	void powerUpWhip(bool upgrade = true);
	Whip* getWhip() { return whip; }
	int getHp() { return hp; };
	int getEnergy() { return energy; };
private:
	bool isHitting;
	bool isThrowing;
	bool isCollectingWhip;
	Timer* collectingWhipTimer = new Timer(SIM_POWERING_DURATION);
	Timer* throwingTimer = new Timer(SIM_DELTA_TRHOWING_TIME);
	bool isReleaseSitButton;
	Whip* whip;
	SubWeapon* subWeapon;
	int subWeaponType;
	bool isInGround;
	int forceRenderFrame;
	int energy;

	/*----------------- simon actions -----------------*/
	void move(int side);
	void jump();
	void sit();
	void stand();
	void standUp();
	void stopMoveWhenHitting();
	void hit();
	void hitWhenSitting();
	bool canThrow();
	void throwing();
	void throwingWhenSitting();
	void throwSubWeapon();
	void loseEnergy() { energy--; energy = energy < 0 ? 0 : energy; };
	void addEnergy() { energy++; energy = energy > MAX_ENERGY ? MAX_ENERGY : energy; };
	void generateSubWeapon();
	bool isHaveSubWeapon() const { return subWeaponType != -1; };
	bool isPowering() const { return !collectingWhipTimer->IsTimeUp(); };


	/*----------------- check collision -----------------*/
	void processCollisionWithGround(float minTy, float ny);
	void processCollisionWithBoundaryByX(float minTx, float ny);
	void checkCollisionWithBoundary(DWORD dt, vector<LPGAMEOBJECT>* boundaries);
	void checkCollisionWithItems(DWORD dt, vector<GameObject*>* items);
	void upgradeWhipLv(bool up = true) const;
	void updateWeaponAction(DWORD dt, vector<GameObject*>* objs);
	void updateRGB();
	void checkCollision(DWORD dt, const vector<MapGameObjects>& map);

	void processCollisionWithItem(Item* item);
	void processWithBoundary(float minTx, float minTy, float nx, float ny);
};


