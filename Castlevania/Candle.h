#pragma once
#include "Grid.h"
#include "ItemFactory.h"
#include "Timer.h"
#include "ItemContainer.h"

enum CandleType
{
	candleBig,
	candleSmall
};

class Candle : public ItemContainer
{
public:
	void init();
	Candle();
	~Candle();
	void initAnim() override;
	void update(DWORD dt, vector<GameObject*>* coObjects) override;

	int CandleType() const;
	void setCandleType(int val);
	Box getBoundingBox() override;
	void render() override;
private:
	int candleType;
};
