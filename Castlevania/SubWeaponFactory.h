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

	int getDmg(int type);
	SubWeapon* getSubWeapon(int type);
	SubWeapon* getSubWeapon(int type, int faceSide);
	D3DXVECTOR2 getVelocity(int type);


private:
	SubWeaponFactory();
	static SubWeaponFactory* instance;
};

