#pragma once
#include "Candle.h"

class SmallCandle :
	public Candle
{
public:
	SmallCandle();
	~SmallCandle() = default;
};

inline SmallCandle::SmallCandle()
{
	type = smallCandle;
	animId = type;
}
