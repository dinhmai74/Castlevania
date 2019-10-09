#pragma once
#include "Weapon.h"

class SubWeapon :
	public Weapon
{
public:
	SubWeapon();
	void init();
	void initAnim() override;

	/*----------------- get set -----------------*/
	~SubWeapon() = default;
	void setWeaponType(int val) { weaponType = val; }
	D3DXVECTOR2 getInitFireVelocity() const { return initFireVelocity; }
	void setInitFireVelocity(D3DXVECTOR2 val) { initFireVelocity = val; }
	int getWeaponType() const { return weaponType; }
	D3DXVECTOR2 acceleration;

	void update(DWORD dt, D3DXVECTOR2 simonPos, int simonState, vector<GameObject*>* coObjects) override;
protected:
	bool fired;
	int weaponType;
	D3DXVECTOR2 initFireVelocity;
};
