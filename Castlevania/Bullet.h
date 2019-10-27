﻿#pragma once
#include "GameObject.h"

enum BulletType
{
	BulletFireball
};

class Bullet: public GameObject
{
public:
	virtual ~Bullet();
	void initAnim() override;
	Bullet();
	void update(DWORD dt, vector<LPGAMEOBJECT>* coObject) override;
	void updateSpeed();
	int getDmg() { return 2; }

protected:
	int ny;
	D3DXVECTOR2 velocity;
};

