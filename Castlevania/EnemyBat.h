#pragma once
#include "Enemy.h"

class EnemyBat final : public Enemy
{
public:
	EnemyBat();
	~EnemyBat();
	void initAnim() override;
	void update(DWORD dt, vector<GameObject*>* coObjects) override;
	void updateVy();

	D3DXVECTOR2 getPosBaseOnSimon(int& nx);
	virtual void generateEnemy(float playerX, float playerY) override;


	Box getBoundingBox() override;
	bool getHurt(int nx, int ny, int hpLose) override;
private:
	Timer* timerAddVelocityY = new Timer(1000);
	bool isBottom;
	bool isTop;
};
