#include "WhipItem.h"
#include "StageManager.h"

WhipItem::WhipItem(): Item()
{
	itemType = ItemType::whipItem;
}

void WhipItem::update(DWORD dt, vector<LPGameObject>* bricks)
{
	Item::update(dt, bricks);
}

void WhipItem::render()
{
	if (currentState != State::dead)animations[ANIM_WHIP_ITEM_IDLE]->render(1,x, y);
	else
	{
		StageManager::getInstance()->remove(this);
	}
}

RECT WhipItem::getBoundingBox()
{
	return GameObject::getBoundingBox(WHIP_ITEM_W, WHIP_ITEM_H);
}

