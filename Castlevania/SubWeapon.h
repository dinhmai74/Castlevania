#pragma once
#include "Weapon.h"

class SubWeapon :
	public Weapon 
{
	int weaponType;
	D3DXVECTOR2 initFireVelocity;
public:
	SubWeapon();
	void init();
	void initAnim() override;

	/*----------------- get set -----------------*/
	~SubWeapon() = default;
	int WeaponType() const { return weaponType; }
	void setWeaponType(int val) { weaponType = val; }
	D3DXVECTOR2 getInitFireVelocity() const { return initFireVelocity; }
	void setInitFireVelocity(D3DXVECTOR2 val) { initFireVelocity = val; }

	virtual void update(DWORD dt, D3DXVECTOR2 simonPos, int simonState, vector<GameObject*>* coObjects) override;
	void setGravity();
private:
	bool fired;
public:
	void setAcceleration(const  D3DXVECTOR2 velocity);
	D3DXVECTOR2 acceleration;
};

