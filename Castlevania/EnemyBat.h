﻿#pragma once
#include "Enemy.h"

class EnemyBat final : public Enemy
{
public:
	EnemyBat();
	~EnemyBat();
	void initAnim() override;
	void update(DWORD dt, vector<GameObject*>* coObjects) override;
	void updateVy();

	virtual void generateEnemy(float playerX, float playerY) override;

private:
	Timer* timerAddVelocityY = new Timer(1000);
	bool isBottom;
	bool isTop;
};
