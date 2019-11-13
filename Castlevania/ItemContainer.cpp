#include "ItemContainer.h"
#include "ItemFactory.h"
#include "StageManager.h"

void ItemContainer::generateItem() {
	if (!isGeneratedItem && itemId !=-1) {
		auto itemY = y;
		const auto item = ItemFactory::Get()->getItem(getItemId(), { x, itemY });
		item->setFaceSide(getItemNx());
		item->setPos(x, itemY);
		item->setInitPos({ x,itemY });
		StageManager::getInstance()->add(item);
		isGeneratedItem = true;
	}
}
ItemContainer::ItemContainer() {
	itemId = -1;
	isGeneratedItem = false;
}
