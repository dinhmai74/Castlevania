#pragma once
#include "ItemContainer.h"
#include "BrokenWallFragment.h"

enum BrokenWallState {
	Init,
	Broken,
	Broking,
};

class BrokenWall: public ItemContainer
{
public:
	BrokenWall();
	BrokenWall(float x,float y);
	~BrokenWall();
	
	void init(float x, float y) ;
	void initAnim() override;

	void render() override;
	void update(DWORD dt, vector<GameObject*>* coObjects) override;
	void updateAnimId() override;
	bool getHurt(int nx, int ny, int hpLose) override;
	Box getBoundingBox() override;


private:
	Timer* timerActive = new Timer();
	vector<BrokenWallFragment*> fragments;
};
