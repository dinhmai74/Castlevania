#include "ItemContainer.h"
#include "ItemFactory.h"
#include "StageManager.h"

void ItemContainer::generateItem() {
	if (!isGeneratedItem) {
		auto itemY = y;
		const auto item = ItemFactory::Get()->getItem(getItemId(), { x, itemY });
		item->setFaceSide(getItemNx());
		item->setPos(x, itemY);
		item->setInitPos({ x,itemY });
		StageManager::getInstance()->add(item);
		isGeneratedItem = true;
	}
}
ItemContainer::ItemContainer() {}
