#pragma once
#include "GameObject.h"

class CollisionEffect: public GameObject
{
public:
	CollisionEffect();
	~CollisionEffect();
	void render() override;
	void initAnim() override;

	virtual RECT getBoundingBox() override;

};

