#pragma once
#include "SubWeapon.h"

class SubWeaponAxe final : public SubWeapon
{
	
public:
	virtual void update(DWORD dt, D3DXVECTOR2 simonPos, int simonState, vector<GameObject*>* coObjects) override
	{
		// sub update
		DebugOut(L"update\n");
		SubWeapon::update(dt, simonPos, simonState, coObjects);
	}

	SubWeaponAxe();
	~SubWeaponAxe()= default;
};
