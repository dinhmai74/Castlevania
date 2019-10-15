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
	virtual void respawn(float playerX, float playerY);

	void generateEnemy(float playerX);
	void processWhenBurnedEffectDone() override;
	void render() override;

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


	void setRespawnTime(DWORD val) { respawnTime = val;  }
	Timer* getTimerRespawn() const { return timerRespawn; }
	void setTimerRespawn(Timer* val) { timerRespawn = val; }
	bool getForceRespawn() const { return forceRespawn; }
	void setForceRespawn(bool val) { forceRespawn = val; }
	bool getIsVirgin() const { return isVirgin; }
	void setIsVirgin(bool val=true) { isVirgin = val; }
private:
	int enemyType{};
	bool forceRespawn;
	bool isVirgin=true;
	int dmg{};
	int initFaceSide{};
	bool isExist{};
	bool isStopAllAction{};
	DWORD respawnTime{};
	Timer* timerRespawn= new Timer(1000);
	Region respawnArea{};

};
