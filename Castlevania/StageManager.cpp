#include "StageManager.h"

StageManager* StageManager::instance = nullptr;


StageManager::~StageManager()
{
}

void StageManager::init(vector<TileMapInfo> tileMapsInfo)
{
	this->tileMapsInfo = tileMapsInfo;

	loadTileMaps();
	currentStage = new Stage();
	getCurrentStage()->init(tileMapsInfo[0].id, tileMapsInfo[0].mapName);
}


void StageManager::loadTileMaps()
{
	for (auto&& map : tileMapsInfo)
	{
		TileMapManager::getInstance()->add(map);
	}
}


void StageManager::add(GameObject* ob) const
{
	auto pos = ob->getPosition();
	auto unit = new Unit(getCurrentStage()->getGrid(), ob, pos.x, pos.y);
}
