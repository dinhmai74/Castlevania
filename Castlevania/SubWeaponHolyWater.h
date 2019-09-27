#pragma once
#include "SubWeapon.h"

class SubWeaponHolyWater : public SubWeapon
{
public:
	SubWeaponHolyWater();
	~SubWeaponHolyWater() = default;
	void update(DWORD dt, D3DXVECTOR2 simonPos, int simonState, vector<GameObject*>* coObjects) override;
	void updateAnimId() override;
	void processWithBoundary(GameObject* const object, float nx, float ny);
	void checkCollision(DWORD dt, vector<GameObject*>* coObjs) override;
	void checkCollisionWithBoundary(DWORD dt, vector<GameObject*>* coObjs);
private:
	Timer* timerEffect;
	bool isBurned;
	DWORD startEffect;
	void updateEffect();
};
