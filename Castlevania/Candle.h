#pragma once
#include "GameObject.h"
#include "Grid.h"
#include "ItemFactory.h"

class Candle : public GameObject
{
public:
	Candle() { initAnim(); animId = bigCandle; }
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
			const auto item = ItemFactory::Get()->getItem(itemInside, { x,y });
			item->setFaceSide(itemNx);
			auto unit = new Unit(grid, item, x, y);
			isGeneratedItem = true;
		}
	};
private:
	bool isGeneratedItem;
};

inline void Candle::initAnim()
{
	addAnimation(bigCandle, "bigcandle_ani");
	addAnimation(smallCandle, "smallcandle_ani");
}

inline void Candle::update(DWORD dt, vector<GameObject*>* coObjects)
{
	GameObject::update(dt);
	GameObject::checkCollisionAndStopMovement(dt, coObjects);
}
