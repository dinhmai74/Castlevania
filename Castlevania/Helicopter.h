#pragma once
#include "GameObject.h"

class Helicopter: public GameObject
{
public:
	Helicopter();
	~Helicopter();
	void initAnim() override;
	void update(DWORD dt, vector<GameObject*>* coObjects= nullptr) override;
};


