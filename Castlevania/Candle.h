#pragma once
#include "GameObject.h"
#include "Grid.h"
#include "ItemFactory.h"
#include "Timer.h"

class Candle : public GameObject
{
public:
	void init();
	Candle() { init(); }
	~Candle() = default;
	void initAnim() override;
	void update(DWORD dt, vector<GameObject*>* coObjects) override;
	int itemInside;
	Grid* grid;
	int itemNx;

	void generateItem()
	{
		if (!isGeneratedItem && grid)
		{
			auto box = getBoundingBox();
			auto itemY = y + (box.bottom - box.top) / 2 - 20;
			const auto item = ItemFactory::Get()->getItem(itemInside, { x,itemY });
			item->setFaceSide(itemNx);
			auto unit = new Unit(grid, item, x, itemY);
			isGeneratedItem = true;
		}
	}

private:
	bool isGeneratedItem;
};

inline void Candle::init()
{
	initAnim();
	animId = bigCandle;
}

inline void Candle::initAnim()
{
	addAnimation(bigCandle, "bigcandle_ani");
	addAnimation(smallCandle, "smallcandle_ani");
}

inline void Candle::update(DWORD dt, vector<GameObject*>* coObjects)
{
	GameObject::update(dt);
	GameObject::checkCollisionAndStopMovement(dt, coObjects);
	if (burnEffect && burnEffect->isOver(BURNED_DURATION - 100)) generateItem();
}
