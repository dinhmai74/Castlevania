#pragma once
#include "GameObject.h"

class Enemy: public GameObject
{
public:
	Enemy();
	~Enemy();
	void init();
	void reset();
	void resetHP();
	void resetPos();
	void update(DWORD dt, vector<GameObject*>* coObjects = nullptr) override;
	void checkCollisionAndChangeDirectX(DWORD dt, vector<GameObject*>* coObjects);
	void changeDirection(const vector<CollisionEvent*>& vector, float nx, float ny);
	void respawn(float playerX, float playerY);

	bool canRespawn();


	void setNewEnemy(bool val = true) ;
	void getHurt(int nx, int ny, int hpLose) override;
	void processWhenBurnedEffectDone() override;

	/*----------------- get set  -----------------*/
	int getEnemyType() const { return enemyType; }
	void setEnemyType(int val) { enemyType = val; }
	int getDmg() const { return dmg; }
	void setDmg(int val) { dmg = val; }
	int getInitFaceSide() const { return initFaceSide; }
	void setInitFaceSide(int val) { initFaceSide = val; }

private:
	int enemyType;
	int dmg;
	int initFaceSide;
	bool isExist;
	DWORD respawnTime;
	Timer* timerRespawn;
};
