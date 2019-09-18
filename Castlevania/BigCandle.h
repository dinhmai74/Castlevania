#pragma once
#include "Candle.h"

class BigCandle :
	public Candle
{
public:
	BigCandle();
	~BigCandle();

	void initAnim() override;
	Box getBoundingBox() override;
	void render() override;
	void update(DWORD dt, vector<GameObject*>* coObjects) override;
private:
	void init();
};

