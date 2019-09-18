#pragma once
#include "Candle.h"
#include "BigCandle.h"
#include "SmallCandle.h"

class CandleFactory
{
public:
	~CandleFactory() { instance = nullptr; }

	static CandleFactory* Get()
	{
		if (instance == nullptr)
			instance = new CandleFactory;
		return instance;
	}

	static Candle* getCandle(const int type, bool isEnable = true)
	{
		Candle* candle;
		switch (type)
		{
		case bigCandle: candle = new BigCandle(); break;
		case smallCandle: candle = new SmallCandle(); break;
		default: candle = new Candle();
		}
		candle->setEnable(isEnable);
		return candle;
	}

private:
	CandleFactory() = default;
	static CandleFactory* instance;
};

CandleFactory* CandleFactory::instance = nullptr;
