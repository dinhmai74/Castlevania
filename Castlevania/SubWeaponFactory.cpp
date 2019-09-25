#include "SubWeaponFactory.h"
#include "SubWeaponDagger.h"
#include "SubWeaponAxe.h"
#include <stdexcept>

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

D3DXVECTOR2 SubWeaponFactory::getInitVelocity(int type, float faceSide)
{
	switch (type)
	{
	case itemDagger: return { 0.00001f * faceSide, 0 }; break;
	case itemAxe: return { 0.00001f * faceSide, -0.6f }; break;
	default:;
	}
	return { 0,0 };
}

SubWeapon* SubWeaponFactory::getSubWeapon(int type, int faceSide)
{
	auto subWeapon = new SubWeapon();
	const auto velocity = getVelocity(type, FLOAT(faceSide));
	const auto initVelo = getInitVelocity(type, FLOAT(faceSide));
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

	subWeapon->setAcceleration(initVelo);
	subWeapon->setGravity();
	subWeapon->setSpeed(velocity.x, velocity.y);
	subWeapon->setDmg(dmg);
	subWeapon->setFaceSide(faceSide);
	return subWeapon;
}

D3DXVECTOR2 SubWeaponFactory::getVelocity(int type, float faceSide)
{
	switch (type)
	{
	case itemDagger: return { FLOAT(0.5 * faceSide), 0 }; break;
	case itemAxe: return { 0.3,-0.8f }; break;
	default: break;
	}
	return { 0, 0 };
}

SubWeaponFactory::SubWeaponFactory()
{
}
