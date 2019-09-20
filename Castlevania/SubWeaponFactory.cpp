#include "SubWeaponFactory.h"
#include "SubWeaponDagger.h"

SubWeaponFactory* SubWeaponFactory::instance = nullptr;

SubWeapon* SubWeaponFactory::getSubWeapon(int type)
{
	auto subWeapon = new SubWeapon();
	auto velocity = getVelocity(type);
	switch (type)
	{
	case itemDagger:
		subWeapon = new SubWeaponDagger();
		subWeapon->setSpeed(velocity.x, velocity.y);
		break;
	default: break;
	}
	return subWeapon;
}

D3DXVECTOR2 SubWeaponFactory::getVelocity(int type)
{
	switch (type)
	{
	case itemDagger: return {0.5, 0};
		break;
	default: break;
	}
	return {0, 0};
}

SubWeaponFactory::SubWeaponFactory()
{
}
