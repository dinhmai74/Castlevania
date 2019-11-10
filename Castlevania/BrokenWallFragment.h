#pragma once
#include "GameObject.h"


class BrokenWallFragment: public GameObject
{
public:
	BrokenWallFragment();
	~BrokenWallFragment();
	void initAnim() override;
	void update(DWORD dt, vector<GameObject*>* coObjects) override;
protected:
	
private:
};
