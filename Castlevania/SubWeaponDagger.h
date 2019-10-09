#pragma once
#include "SubWeapon.h"

class SubWeaponDagger final : public SubWeapon
{
public:
	SubWeaponDagger()
	{
		addAnimation(itemDagger, "subweapons_dagger_ani");
		setWeaponType(itemDagger);
		setAnimId(itemDagger);
		vx = 0.5f;
	};

	~SubWeaponDagger() = default;
};
