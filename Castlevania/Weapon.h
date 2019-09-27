#pragma once
#include "GameObject.h"
#include "Candle.h"

class Weapon : public GameObject
{
	int dmg; // dmg that weapon can do;
	int remainHit; // if = -1 => infinite weapon
public:
	explicit Weapon(int dmg = 1)
		: dmg(dmg)
	{
		remainHit = 1;
	}

	void setRemainHit(int val) { remainHit = val; };
	void initAnim() override {};
	void setDmg(int val) { dmg = val; }
	int getDmg() const { return dmg; }
	void disableWeapon();
	void processWithCandle(GameObject* coObject, int nx = 1, int ny = 1);
	virtual void checkCollision(DWORD dt, vector<GameObject*>* coObjs);
	virtual void update(DWORD dt, D3DXVECTOR2 simonPos, int simonState, vector<GameObject*>* coObjects);
};