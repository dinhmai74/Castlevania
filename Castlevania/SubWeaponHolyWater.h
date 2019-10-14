#pragma once
#include "SubWeapon.h"

class SubWeaponHolyWater : public SubWeapon
{
public:
	SubWeaponHolyWater();
	~SubWeaponHolyWater() = default;
	void update(DWORD dt, D3DXVECTOR2 simonPos, int simonState, vector<MapGameObjects>& maps);
	void updateAnimId() override;
	void checkCollision(DWORD dt, vector<MapGameObjects>& coObjs) ;
	CollisionResult checkCollisionWithBoundary(DWORD dt, vector<GameObject*>* coObjs) ;
private:
	Timer* timerEffect;
	bool isBurned;
	DWORD startEffect;
	void updateEffect();
	void checkCollisionWithEnemy(DWORD dt, vector<GameObject*>* objs);
};
