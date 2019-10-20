#pragma once
#include "Enemy.h"

class EnemyVampireBoss final : public Enemy
{
public:
	EnemyVampireBoss();
	~EnemyVampireBoss();
	void init() override;
	void initAnim() override;

	void update(DWORD dt, vector<GameObject*>* coObjects = nullptr) override;
	void checkCanAwake();
	void checkAttack();
	void updateVelocityWhenAttack();
private:
	GameObject* simon;
};
