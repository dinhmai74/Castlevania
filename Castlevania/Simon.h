#pragma once
#include "GameObject.h"
#include "Whip.h"
#include "Constants.h"
#include "SetTimeOut.h"
#include <conio.h>

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
	void update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);

	virtual void render() override;
	void updateAnimId();
	void initAnim() override;
	Box getBoundingBox() override;
	void renderBoundingBox() override;
	void upgradeWhipLv(bool up = true) const;

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
	void checkCollision(DWORD dt, vector<LPGAMEOBJECT>* coObject);
	//void processCollisionWithItem(Item* item);
};
