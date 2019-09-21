#include "SubWeaponFactory.h"
#include "SubWeaponDagger.h"

SubWeaponFactory* SubWeaponFactory::instance = nullptr;

int SubWeaponFactory::getDmg(int type)
{
	switch (type)
	{
	case itemDagger: return 1;
	default: return 1;
	}
}

SubWeapon* SubWeaponFactory::getSubWeapon(int type)
{
	return getSubWeapon(type, 1);
}

SubWeapon* SubWeaponFactory::getSubWeapon(int type, int faceSide)
{
	auto subWeapon = new SubWeapon();
	const auto velocity = getVelocity(type);
	auto dmg = getDmg(type);
	switch (type)
	{
	case itemDagger:
		subWeapon = new SubWeaponDagger();
		break;
	default: break;
	}

	subWeapon->setSpeed(velocity.x * faceSide, velocity.y);
	subWeapon->setDmg(dmg);
	subWeapon->setFaceSide(faceSide);
	return subWeapon;
}

D3DXVECTOR2 SubWeaponFactory::getVelocity(int type)
{
	switch (type)
	{
	case itemDagger: return { 0.5, 0 };
					 break;
	default: break;
	}
	return { 0, 0 };
}

SubWeaponFactory::SubWeaponFactory()
{
}
