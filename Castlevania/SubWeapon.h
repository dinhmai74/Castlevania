#pragma once
#include "GameObject.h"
class SubWeapon :
	public GameObject
{
public:
	SubWeapon();
	void init();
	void initAnim() override;
	void update(DWORD dt, vector<GameObject*>* coObjects) override;
	~SubWeapon() = default;
};

