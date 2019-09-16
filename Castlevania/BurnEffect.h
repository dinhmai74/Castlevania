#pragma once
#include "GameObject.h"

class BurnEffect: public GameObject
{
public:
	BurnEffect();
	~BurnEffect();
	void render() override;
	void update(DWORD dt, vector<GameObject*>* coObjects) override;
	void initAnim() override;

	virtual RECT getBoundingBox() override;

};

