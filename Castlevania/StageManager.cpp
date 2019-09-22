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

void StageManager::nextStage(int id)
{
	auto newStage = new Stage;
	// cause map id from =1 so next one id from tileMapsInfo is this
	auto nextId = id == -1 ? currentStage->getId() : id;
	newStage->init(tileMapsInfo[nextId].id, tileMapsInfo[nextId].mapName);
	setStage(newStage);
}


void StageManager::loadTileMaps()
{
	for (auto&& map : tileMapsInfo)
	{
		TileMapManager::getInstance()->add(map);
	}
}


void StageManager::onKeyDown(int keyCode) 
{
	getCurrentStage()->onKeyDown(keyCode);
	switch (keyCode)
	{
	case DIK_N: nextStage();break;
	case DIK_Q: nextStage(0); break;
	case DIK_W: nextStage(1); break;

	default:
		break;
	}
}

void StageManager::add(GameObject* ob) const
{
	auto pos = ob->getPosition();
	auto unit = new Unit(getCurrentStage()->getGrid(), ob, pos.x, pos.y);
}
