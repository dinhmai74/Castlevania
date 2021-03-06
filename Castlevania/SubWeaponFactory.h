﻿#pragma once
#include "SubWeapon.h"

class SubWeaponFactory
{
public:
	~SubWeaponFactory() { instance = nullptr; }

	static SubWeaponFactory* getInstance()
	{
		if (instance == nullptr)
			instance = new SubWeaponFactory;
		return instance;
	}

	SubWeapon* getSubWeapon(int type);
	SubWeapon* getSubWeapon(int type, int faceSide);

private:
	SubWeaponFactory();
	static SubWeaponFactory* instance;
};
