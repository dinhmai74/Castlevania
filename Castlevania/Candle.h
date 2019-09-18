#pragma once
#include "GameObject.h"

class Candle : public GameObject
{
public:
	Candle() { initAnim(); animationId = bigCandle; }
	~Candle() = default;
	void initAnim() override;
	void render() override;
	void update(DWORD dt, vector<GameObject*>* coObjects) override;
	int itemInside;
};

inline void Candle::initAnim()
{
	addAnimation(bigCandle, "bigcandle_ani");
	addAnimation(smallCandle, "smallcandle_ani");
}

inline void Candle::render()
{
	GameObject::render();

	if (!IsEnable()) return;

	animations[animationId]->render(faceSide, x, y);
}

inline void Candle::update(DWORD dt, vector<GameObject*>* coObjects)
{
	GameObject::update(dt);
	GameObject::checkCollisionAndStopMovement(dt, coObjects);
}
