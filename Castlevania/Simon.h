#pragma once
#include "GameObject.h"
#include "Whip.h"
#include "Constants.h"
#include "SetTimeOut.h"
#include <conio.h>
#include "Item.h"

class Simon :
	public GameObject
{
public:
	Simon();

	void init();

	~Simon();

	void handleOnKeyPress(BYTE* states);
	void handleOnKeyDown(int KeyCode);
	void handleOnKeyRelease(int KeyCode);

	void resetState();


	// Inherited via GameObject
	void setState(int state);

	virtual void render() override;
	void updateAnimId();
	void initAnim() override;
	Box getBoundingBox() override;
	void renderBoundingBox() override;
	void upgradeWhipLv(bool up = true) const;
	void update(DWORD dt, const vector<MapGameObjects>& maps);

private:
	bool isHitting;
	bool isThrowing;
	bool isReleaseSitButton;
	Whip* whip;
	bool isInGround;

	/*----------------- simon actions -----------------*/
	void move(int side);
	void jump();
	void sit();
	void stand();
	void standUp();
	void hit();
	void hitWhenSitting();
	void throwing();
	void throwingWhenSitting();
	void throwSubWeapon();

	/*----------------- check collision -----------------*/
	void processCollisionWithGround(float minTy, float ny);
	void processCollisionWithBoundaryByX(float minTx, float ny);
	void checkCollisionWithBoundary(DWORD dt, vector<LPGAMEOBJECT>* boundaries);
	void checkCollisionWithItems(DWORD dt, vector<GameObject*>* items);
	void updateWeaponAction(DWORD dt, vector<GameObject*>* objs);
	void checkCollision(DWORD dt, const vector<MapGameObjects>& map);
	void processCollisionWithItem(Item* item) const;
};

