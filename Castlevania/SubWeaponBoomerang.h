#pragma once
#include "SubWeapon.h"
#include "Territory.h"

class SubWeaponBoomerang final : public SubWeapon
{
	Territory territory;
	bool changedDirection;
	float firedDistance;
public:
	SubWeaponBoomerang();
	~SubWeaponBoomerang() = default;
	void update(DWORD dt, D3DXVECTOR2 simonPos, int simonState, vector<GameObject*>* coObjects) override;
	void checkCollision(DWORD dt, vector<GameObject*>* coObjs) override;
	void processCollisionWithSimon(GameObject* object, float nx, float ny);
};
