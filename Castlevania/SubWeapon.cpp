#include "SubWeapon.h"
#include "Candle.h"

SubWeapon::SubWeapon()
{
	init();
}

void SubWeapon::init()
{
	type = subWeapon;
	initAnim();
}

void SubWeapon::initAnim()
{
	addAnimation(itemDagger, "subweapons_dagger_ani");
	addAnimation(itemAxe, "subweapons_axe_ani");
	addAnimation(itemCross, "subweapons_cross_ani");
}

void SubWeapon::update(DWORD dt, D3DXVECTOR2 simonPos, int simonState, vector<GameObject*>* coObjects)
{
	Weapon::update(dt, simonPos, simonState, coObjects);
	if (!fired)
	{
		vx = initFireVelocity.x;
		vy = initFireVelocity.y;
		fired = true;
		DebugOut(L"init\n");
	}
	else {
		vx += acceleration.x;
		vy += acceleration.y - gravity;
	}
}

void SubWeapon::setAcceleration(const D3DXVECTOR2 velocity)
{
	this->acceleration = velocity;
}
