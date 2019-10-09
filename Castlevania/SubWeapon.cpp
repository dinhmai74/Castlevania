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
	updateGravity(gravity);
	Weapon::update(dt, simonPos, simonState, coObjects);
}

void SubWeapon::setAcceleration(const D3DXVECTOR2 velocity)
{
	this->acceleration = velocity;
}