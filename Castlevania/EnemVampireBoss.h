#pragma once
#include "ShootingEnemy.h"

auto constexpr RANDOMSPOT_AREA = 200;
auto constexpr MISS_ATTACK_RATE = 20;
auto constexpr GET_NEW_POS_RATE = 50;

class EnemyVampireBoss final : public ShootingEnemy
{
public:
	EnemyVampireBoss();
	~EnemyVampireBoss();
	void init() override;
	void initAnim() override;

	void update(DWORD dt, vector<GameObject*>* coObjects = nullptr) override;
	void checkCanAwake();
	void getNextPositionToFly();
	void updateVelocity();
	Box getBoundingBox() override;
	void processDeathEffect() override;
	void setState(int state) override;
private:
	int nx;
	int ny;
	Timer* timerShooting = new Timer(1000);
	GameObject* simon;
	D3DXVECTOR2 nextTargetPos;
	D3DXVECTOR2 safePos;
	D3DXVECTOR2 getRandomPosInBound();
	Timer* timerActionCountDown = new Timer(2000);
	Timer* timerIdle = new Timer(1000);
	bool isHitDone;
	DWORD deathTime;
	void setIdle();
	void getNewActionBaseOnState();
	// over 1000;
	int missrate;
	float shootingRegion;
	void shoot() override;

	virtual bool canShoot() override;

	virtual void generateBullet() override;
	D3DXVECTOR2 getRandomPosBaseOnSim();
	void updateDirection();
};