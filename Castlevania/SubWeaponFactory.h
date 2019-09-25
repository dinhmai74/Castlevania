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

	int getDmg(int type);
	SubWeapon* getSubWeapon(int type);
	D3DXVECTOR2 getVelocity(int type, float faceSide);
	SubWeapon* getSubWeapon(int type, int faceSide);
	float getGravity(int type);

private:
	SubWeaponFactory();
	static SubWeaponFactory* instance;
};

