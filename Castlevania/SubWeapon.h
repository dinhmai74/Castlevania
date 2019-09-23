#pragma once
#include "Weapon.h"

class SubWeapon :
	public Weapon 
{
	int weaponType;
public:
	SubWeapon();
	void init();
	void initAnim() override;

	/*----------------- get set -----------------*/
	~SubWeapon() = default;
	int WeaponType() const { return weaponType; }
	void setWeaponType(int val) { weaponType = val; }
};

