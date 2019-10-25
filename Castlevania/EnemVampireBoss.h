#pragma once
#include "Enemy.h"

auto constexpr RANDOMSPOT_AREA= 200;

class EnemyVampireBoss final : public Enemy
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
	void chaseTarget();
	Box getBoundingBox() override;
	void processDeathEffect() override;
	void setState(int state) override;
private:
	GameObject* simon;
	D3DXVECTOR2 nextTargetPos;
	D3DXVECTOR2 getRandomPosInBound();
	Timer* timerActionCountDown = new Timer(2000);
	Timer* timerIdle = new Timer(1000);
	bool isHitDone;
	DWORD deathTime;
	void setIdle();
	void getNewActionBaseOnState();
};
