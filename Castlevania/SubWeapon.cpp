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
	addAnimation(itemBoomerang, "subweapons_cross_ani");
	addAnimation(itemHolyWater, "subweapons_holy_water_ani");
	addAnimation(itemHolyWaterEffect, "subweapons_holy_water_effect_ani");
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