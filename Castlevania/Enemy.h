#pragma once
#include "GameObject.h"
#include "Territory.h"

enum EnemType
{
	EnemGhouls,
	EnemWolf,
};


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
	bool canRespawn(D3DXVECTOR2 simPos);
	void respawn(float playerX, float playerY);

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

	Region getRespawnArea() const { return respawnArea; }
	void setRespawnArea(Region val) { respawnArea = val; }

	virtual void setEnable(bool val = true) override;

	bool getIsStopAllAction() const { return isStopAllAction; }
	void setIsStopAllAction(bool val) { isStopAllAction = val; }

	virtual void render() override;

	void setRespawnTime(DWORD val) { respawnTime = val; }
private:
	int enemyType;
	int dmg;
	int initFaceSide;
	bool isExist;
	bool isStopAllAction;
	DWORD respawnTime;
	Timer* timerRespawn;
	Region respawnArea;

};
