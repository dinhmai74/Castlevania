#pragma once
#include "Candle.h"

class BigCandle :
	public Candle
{
public:
	BigCandle();
	~BigCandle()= default;
};

inline BigCandle::BigCandle()
{
	type = bigCandle;
	animId = type;
}

