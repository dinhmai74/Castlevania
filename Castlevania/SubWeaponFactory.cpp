#include "SubWeaponFactory.h"
#include "SubWeaponDagger.h"
#include "SubWeaponAxe.h"
#include "SubWeaponBoomerang.h"
#include "SubWeaponHolyWater.h"

SubWeaponFactory* SubWeaponFactory::instance = nullptr;

SubWeapon* SubWeaponFactory::getSubWeapon(int type)
{
	return getSubWeapon(type, 1);
}
SubWeapon* SubWeaponFactory::getSubWeapon(int type, int faceSide)
{
	auto subWeapon = new SubWeapon();
	D3DXVECTOR2 velocity = { 0,0 };
	auto dmg = 1;
	float gravity = 0;
	auto hits = 1;

	switch (type)
	{
	case itemDagger:
		subWeapon = new SubWeaponDagger();
		velocity = { faceSide * 0.4f, 0 };
		break;
	case itemAxe:
		subWeapon = new SubWeaponAxe();
		velocity = { faceSide * 0.13f, -0.4f };
		gravity = 0.0009f;
		break;
	case itemBoomerang:
		subWeapon = new SubWeaponBoomerang();
		velocity = { faceSide * 0.3f, 0 };
		hits = -1;
		break;
	case itemHolyWater:
		subWeapon = new SubWeaponHolyWater();
		velocity = { faceSide * 0.2f, -0.2f };
		gravity = 0.0009f;
		break;
	default: break;
	}

	subWeapon->setSpeed(velocity.x, velocity.y);
	subWeapon->setGravity(gravity);
	subWeapon->setDmg(dmg);
	subWeapon->setRemainHit(hits);
	subWeapon->setFaceSide(faceSide);
	return subWeapon;
}

SubWeaponFactory::SubWeaponFactory()
{
}
