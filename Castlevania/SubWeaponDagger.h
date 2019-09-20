#pragma once
#include "SubWeapon.h"

class SubWeaponDagger final : public SubWeapon
{
public:
	SubWeaponDagger()
	{

		animId = itemDagger;
		vx = 0.5f;
	};

	~SubWeaponDagger() = default;
};

