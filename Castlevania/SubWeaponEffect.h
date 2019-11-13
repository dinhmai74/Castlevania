#pragma once
#include "SubWeapon.h"


class SubWeaponEffect: public SubWeapon
{
public:
	SubWeaponEffect();
	~SubWeaponEffect();
	void render() override;
	void update(DWORD dt, vector<GameObject*>* coObjects) override;
protected:
	
private:
};
