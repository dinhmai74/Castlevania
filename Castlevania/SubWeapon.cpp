#include "SubWeapon.h"

SubWeapon::SubWeapon()
{
	init();
}

void SubWeapon::init()
{
	initAnim();
}

void SubWeapon::initAnim()
{
	addAnimation(itemDagger, "subweapons_dagger_ani");
}

void SubWeapon::update(DWORD dt, vector<GameObject*>* coObjects)
{
	GameObject::update(dt);
}
