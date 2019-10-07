#pragma once
#include "Candle.h"

class BigCandle :
	public Candle
{
public:
	BigCandle();
	~BigCandle() = default;
};

inline BigCandle::BigCandle()
{
	setCandleType(candleBig);
	setAnimId(candleBig);
}
