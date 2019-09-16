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
	currentStage->init(tileMapsInfo[0].id,tileMapsInfo[0].mapName);
}

void StageManager::loadTileMaps()
{
	for (auto&& map: tileMapsInfo)
	{
		TileMapManager::getInstance()->add(map);

	}
}
