#pragma once
#include "GameObject.h"
#include "Territory.h"

enum EnemType {
	EnemGhouls,
	EnemWolf,
};

auto constexpr E_UNTOUCHABLE_DURATION = 200;
auto constexpr E_RESPAWN_INIT_TIME = 3000;

class Enemy : public GameObject {
public:
	Enemy();
	~Enemy();
	virtual void init();
	virtual void reset();
	void resetHp();
	void resetPos();
	void update(DWORD dt, vector<GameObject*>* coObjects = nullptr) override;
	void checkCollisionAndChangeDirectX(DWORD dt, vector<GameObject*>* coObjects);
	void changeDirection(const vector<CollisionEvent*>& vector, float nx, float ny);
	bool canRespawn(D3DXVECTOR2 simPos);
	void respawn(float playerX, float playerY);

	void setNewEnemy(bool val = true);
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

	void setEnable(bool val = true) override;

	bool getIsStopAllAction() const { return isStopAllAction; }
	void setIsStopAllAction(bool val) { isStopAllAction = val; }

	void render() override;

	void setRespawnTime(DWORD val) { respawnTime = val;  }
	Timer* getTimerRespawn() const { return timerRespawn; }
	void setTimerRespawn(Timer* val) { timerRespawn = val; }
private:
	int enemyType{};
	int dmg{};
	int initFaceSide{};
	bool isExist{};
	bool isStopAllAction{};
	DWORD respawnTime{};
	Timer* timerRespawn{};
	Region respawnArea{};

};
