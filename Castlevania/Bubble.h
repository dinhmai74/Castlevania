#pragma once
#include "GameObject.h"


class Bubble final : public GameObject
{
public:
	Bubble();
	~Bubble();
	void initAnim() override;
	void update(DWORD dt, vector<GameObject*>* coObjects) override;
};

