#include "CandleFactory.h"

CandleFactory* CandleFactory::instance = nullptr;

Candle* CandleFactory::getCandle(const int type, const int itemType, const int itemNx, D3DXVECTOR2 pos, Grid* grid, bool isEnable)
{
	Candle* candle;
	switch (type)
	{
	case candleBig: candle = new BigCandle();
		break;
	case candleSmall: candle = new SmallCandle();
		break;
	default: candle = new Candle();
	}
	candle->setEnable(isEnable);
	candle->itemInside = itemType;
	candle->itemNx = itemNx;
	candle->setPosition(pos.x, pos.y);
	candle->grid = grid;
	return candle;
}