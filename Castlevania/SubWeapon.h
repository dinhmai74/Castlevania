#pragma once
#include "Weapon.h"

class SubWeapon :
	public Weapon 
{
public:
	SubWeapon();
	void init();
	void initAnim() override;

	/*----------------- get set -----------------*/
	~SubWeapon() = default;
};

