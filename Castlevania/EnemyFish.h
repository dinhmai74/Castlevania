#pragma once
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

	void updateRespawnPosBaseOnSim();

	void updateWalking();
	bool processCollisionWithGround(float minTy, float ny) override;
	float getJumpingMaxRange() const { return jumpingMaxRange; }
	void setJumpingMaxRange(float val) { jumpingMaxRange = val; }
	void generateEnemy(float playerX, float playerY) override;
	float getActiveRange() const { return activeRange; }
	void setActiveRange(float val) { activeRange = val; }
private:
	float jumpingMaxRange;
	void generateBullet();
	void shoot();
	bool canShoot();
	Timer* timerShooting= new Timer(2000);
	float activeRange;
};
