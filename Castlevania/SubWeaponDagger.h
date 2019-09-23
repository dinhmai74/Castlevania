#pragma once
#include "SubWeapon.h"

class SubWeaponDagger final : public SubWeapon
{
public:
	SubWeaponDagger()
	{
		setWeaponType(itemDagger);
		animId = itemDagger;
		vx = 0.5f;
	};

	~SubWeaponDagger() = default;
};

