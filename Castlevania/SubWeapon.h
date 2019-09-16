#pragma once
#include "GameObject.h"

enum SubWeaponState
{
	active=1,
	inActive=-1
};

class SubWeapon : public GameObject
{
	float cameraX, cameraY;
public:
	SubWeapon();
	~SubWeapon();
	void setSide(int side)
	{
		faceSide = side;
	}
};

