#include "BigCandle.h"
#include "HeartItem.h"
#include "WhipItem.h"
#include "DaggerItem.h"
#include <stdlib.h>
#include "BurnEffect.h"
#include "CollisionEffect.h"
#include "StageManager.h"

BigCandle::BigCandle()
{
	itemContain = -1;
	type = GameObjectType::canHitObject;
	initAnim();
}


BigCandle::~BigCandle()
{
}

void BigCandle::render()
{
	if (currentState != State::dead) animations[ANIM_BIG_CANDLE_IDLE]->render(1,x, y);
	else {
		const auto stageManager = StageManager::getInstance();
		Item * item = nullptr;
		auto burn = new BurnEffect();
		burn->setPosition(x, y);
		stageManager->add(burn);

		auto collisionEffect = new CollisionEffect();
		collisionEffect->setPosition(x,y);
		stageManager->add(collisionEffect);
		if (itemContain == -1)
		{
			itemContain = rand() % 3 + 1;
		}
		if (itemContain == ItemBigCandleContain::heart) {
			item = new HeartItem();
		}
		else if (itemContain == ItemBigCandleContain::whipUpgrade) {
			item = new WhipItem();
		}
		else if (itemContain == ItemBigCandleContain::dagger)
		{
			item = new DaggerItem();
		}

		if (item ) {
			item->setPosition(x, y);
			stageManager->add(item);
		}
		stageManager->remove(this);
	}
}


void BigCandle::setItemContain(int item)
{
	itemContain = item;
}

void BigCandle::initAnim()
{
	addAnimation(ANIM_BIG_CANDLE_IDLE);
}

Box BigCandle::getBoundingBox()
{
	return GameObject::getBoundingBox(BIG_CANDLE_WIDTH, BIG_CANDLE_HEIGHT);
}
