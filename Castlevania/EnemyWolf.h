#pragma once
#include "Enemy.h"

class EnemyWolf final : public Enemy
{
public:
	EnemyWolf();
	~EnemyWolf();

	void initAnim() override;

	void init() override;
	void updateAnimId() override;
	void update(DWORD dt, vector<GameObject*>* coObjects) override;
protected:
	
private:
};
