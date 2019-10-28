#pragma once
#include "Bubble.h"

class Bubbles final : public GameObject
{
public:
	void init(float x, float y);
	Bubbles();
	Bubbles(float x, float y);
	~Bubbles();
	void initAnim() override;

	void render() override;

	void update(DWORD dt, vector<GameObject*>* coObjects = nullptr) override;

private:
	vector<Bubble*> bubbles;
	Timer* timerActive = new Timer(500);
};
