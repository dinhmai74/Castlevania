#pragma once
#include "GameObject.h"
#include "Whip.h"
#include "Constants.h"
#include "SetTimeOut.h"
#include <conio.h>
//states
enum SimonState
{
	idle= 1,
	walking=2,
	sitting=3,
	jumping=4,
	hitting=5,
	hittingWhenSitting=6,
	throwing=7,
	throwingWhenSitting=8
};

class Simon :
	public GameObject
{
	bool isHitting{};
	bool isThrowing{};
	bool isReleaseSitButton;
	bool isOneTimeAnim{};
	Whip* whip;
	bool isInGround;
public:
	Simon();
	~Simon();

	void handleOnKeyPress(BYTE *states);
	void handleOnKeyDown(int KeyCode );
	void handleOnKeyRelease(int KeyCode);
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
	void beginFight();
	void resetState();

	
	// Inherited via GameObject
	void setState(int state);
	void update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = nullptr) ;
	void processCollisionWithGround(float minTy, float ny);
	void processCollisionWithBoundaryByX(float minTx, float ny);
	void checkCollision(DWORD dt, vector<LPGAMEOBJECT> *coObject);
	//void processCollisionWithItem(Item* item);

	virtual void render() override;
	void updateAnimId();

	void initAnim() override;
	RECT getBoundingBox() override;
};


