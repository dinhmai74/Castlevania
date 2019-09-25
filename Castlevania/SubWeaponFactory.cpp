#include "SubWeaponFactory.h"
#include "SubWeaponDagger.h"
#include "SubWeaponAxe.h"
#include <stdexcept>

SubWeaponFactory* SubWeaponFactory::instance = nullptr;


float SubWeaponFactory::getGravity(int type)
{
	switch (type)
	{
	case itemAxe: return 0.0009f;
	default: return 0;
	}
}

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
	const auto velocity = getVelocity(type, FLOAT(faceSide));
	const auto dmg = getDmg(type);
	switch (type)
	{
	case itemDagger:
		subWeapon = new SubWeaponDagger();
		break;
	case itemAxe:
		subWeapon = new SubWeaponAxe();
		break;
	default: break;
	}

	subWeapon->setSpeed(velocity.x, velocity.y);
	subWeapon->setGravity(getGravity(type));
	subWeapon->setDmg(dmg);
	subWeapon->setFaceSide(faceSide);
	return subWeapon;
}

D3DXVECTOR2 SubWeaponFactory::getVelocity(int type, float faceSide)
{
	switch (type)
	{
	case itemDagger: return { 0.5f * faceSide, 0 }; break;
	case itemAxe: return { 0.15f* faceSide,-0.4f }; break;
	default: break;
	}
	return { 0, 0 };
}

SubWeaponFactory::SubWeaponFactory()
{
}
