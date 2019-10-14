#pragma once
#include "Enemy.h"

enum JumpStatus {
	JumpReady,
	JumpFirstTime,
	Jumped
};

class EnemyWolf final : public Enemy
{
public:
	EnemyWolf();
	~EnemyWolf();

	void initAnim() override;

	void init() override;
	void updateAnimId() override;
	void update(DWORD dt, vector<GameObject*>* coObjects,float simX) ;
	void checkIfCanRun(float simX);

	/*----------------- get set -----------------*/
	Territory getActiveTerritory() const { return activeTerritory; }
	void setActiveTerritory(Territory val) { activeTerritory = val; }
	void checkCollisonWithBoundaryForceJump(vector<GameObject*>* coObjects);
	void jump();
	void checkCollisionWithGround(DWORD dt, vector<GameObject*>* coObjects);
	void updateVelocity();

	virtual void reset() override;

private:
	Territory activeTerritory;
	bool canRun;
	int jumpStatus;
	int changeFaceSide;
	bool changedFaceSide;
	bool jumped;
};
