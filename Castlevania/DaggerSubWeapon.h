#pragma once
#include "SubWeapon.h"

constexpr auto DAGGER_W = 32;
constexpr auto DAGGER_H = 18;
constexpr auto DAGGER_VX = 0.3f;

class DaggerSubWeapon :
	public SubWeapon
{
public:
	DaggerSubWeapon();
	~DaggerSubWeapon();
	void checkEnemyCollisions(vector<LPGAMEOBJECT>* coObject);

	void render() override;
	void update(DWORD dt, vector<GameObject*>* coObjects) override;
	void initAnim() override;

	virtual RECT getBoundingBox() override;

};

