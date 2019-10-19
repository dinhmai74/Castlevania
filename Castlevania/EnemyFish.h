﻿#pragma once
#include "Enemy.h"

class EnemyFish final : public Enemy
{
public:
	EnemyFish();
	~EnemyFish();
	void initAnim() override;
	void updateAnimId() override;
	void updateWhenJumping();
	void update(DWORD dt, vector<GameObject*>* coObjects) override;
	void updateVx();
	bool processCollisionWithGround(float minTy, float ny) override;
	float getJumpingMaxRange() const { return jumpingMaxRange; }
	void setJumpingMaxRange(float val) { jumpingMaxRange = val; }
private:
	float jumpingMaxRange;
};