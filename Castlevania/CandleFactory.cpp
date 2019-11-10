#include "CandleFactory.h"

CandleFactory* CandleFactory::instance = nullptr;

Candle* CandleFactory::getCandle(const int type, const int itemType, const int itemNx, D3DXVECTOR2 pos, bool isEnable)
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
	candle->setItemId(itemType);
	candle->setItemNx(itemNx);
	candle->setPos(pos.x, pos.y);
	candle->setInitPos(pos);
	return candle;
}