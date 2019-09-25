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
	auto newStage = new Stage();
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
	case DIK_Q: nextStage(0); break;
	case DIK_W: nextStage(1); break;
	case DIK_E: nextStage(2); break;
	case DIK_R: nextStage(getCurrentStage()->getId() - 1); break;

	default:
		break;
	}
}

void StageManager::add(GameObject* ob) const
{
	auto pos = ob->getPosition();
	auto unit = new Unit(getCurrentStage()->getGrid(), ob, pos.x, pos.y);
}

void StageManager::descreaseLife()
{
	auto simon = currentStage->getSimon();
	const auto result = simon->updateLife(-1);
	if (result) {
		simon->updateHP(SIM_MAX_HP);
		currentStage->reset();
	}
	else
	{
		// TODO: add end game screen
		DebugOut(L"\n end game is real");
	}
}
