#pragma once
#include "SubWeapon.h"

class SubWeaponHolyWater : public SubWeapon
{
public:
	SubWeaponHolyWater();
	~SubWeaponHolyWater() = default;
	void update(DWORD dt, D3DXVECTOR2 simonPos, int simonState, vector<MapGameObjects>& maps);
	void updateAnimId() override;
private:
	Timer* timerEffect;
	bool isBurned;
	DWORD startEffect;
	void checkCollisionWithBoundary(DWORD dt, vector<GameObject*>* coObjs) ;
	void checkCollision(DWORD dt, vector<MapGameObjects>& coObjs) ;
	void processWithBoundary(GameObject* const object, float nx, float ny);
	void updateEffect();
	void checkCollisionWithEnemy(DWORD dt, vector<GameObject*>* objs);
};
