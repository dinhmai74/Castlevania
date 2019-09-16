#pragma once
#include "Game.h"
#include "GameObject.h"

constexpr auto BRICK_BBOX_WIDTH = 16;
constexpr auto BRICK_BBOX_HEIGHT = 16;

class Brick:public GameObject  
{
public:
	Brick();
	~Brick();

	void render() override;

	// Inherited via GameObject

	void update(DWORD dt, vector<GameObject*>* coObjects) override;
	void initAnim() override;

	virtual RECT getBoundingBox() override;

};
