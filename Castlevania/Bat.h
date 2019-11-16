#pragma once

#include "GameObject.h"

class Bat : public GameObject
{
public:
	Bat();
	~Bat();

	void initAnim() override;
	void update(DWORD dt, vector<GameObject*>* coObjects) override;
};

inline void Bat::initAnim() {
	addAnimation(idle,"bat_ani");
}

