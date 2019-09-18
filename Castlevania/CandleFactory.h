#pragma once
#include "Candle.h"
#include "BigCandle.h"
#include "SmallCandle.h"
#include "Grid.h"

class CandleFactory
{
public:
	~CandleFactory() { instance = nullptr; }

	static CandleFactory* Get()
	{
		if (instance == nullptr)
			instance = new CandleFactory();
		return instance;
	}

	static Candle* getCandle(const int type, const int itemType, const int itemNx, D3DXVECTOR2 pos, Grid* grid, bool isEnable = true);

private:
	CandleFactory() = default;
	static CandleFactory* instance;
};

