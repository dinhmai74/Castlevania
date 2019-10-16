#include "SubWeapon.h"
#include "Candle.h"

SubWeapon::SubWeapon()
{
	init();
}

void SubWeapon::init()
{
	type = OBSubWeapon;
	initAnim();
}

void SubWeapon::initAnim()
{
}

void SubWeapon::update(DWORD dt, D3DXVECTOR2 simonPos, int simonState, vector<GameObject*>* coObjects)
{
	Weapon::update(dt, simonPos, simonState, coObjects);
	updatePosWhenNotCollide();
	updateGravity(dt,gravity);
}
