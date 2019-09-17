#pragma once
#include "GameObject.h"
constexpr auto BIG_CANDLE_WIDTH = 32;
constexpr auto BIG_CANDLE_HEIGHT= 64;
constexpr auto BIG_CANDLE_ANIMATION_FRAME_TIME = 100;

enum ItemBigCandleContain
{
	heart=1,
	dagger=2,
	whipUpgrade=3
};

class BigCandle :
	public GameObject
{
	int itemContain;
	bool didInitItemContain;
public:
	BigCandle();
	~BigCandle();

	// Inherited via GameObject
	virtual void render() override;
	void setItemContain(int item);
	void initAnim() override;

	virtual Box getBoundingBox() override;

};

