#pragma once
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
	D3DXVECTOR2 getVelocity(int type);


private:
	SubWeaponFactory();
	static SubWeaponFactory* instance;
};

