#include "StageManager.h"
#include "HUD.h"

StageManager* StageManager::instance = nullptr;

StageManager::~StageManager()
{
}

void StageManager::init(vector<TileMapInfo> tileMapsInfo)
{
	this->tileMapsInfo = tileMapsInfo;

	loadTileMaps();
	currentStage = new Stage();
	currentStage->init(tileMapsInfo[0].id, tileMapsInfo[0].mapName);
}

void StageManager::nextStage(int id)
{
	auto newStage = new Stage();
	// cause map id from =1 so next one id from tileMapsInfo is this
	auto nextId = id == -1 ? currentStage->getId() : id;
	if (nextId > tileMapsInfo.size() - 1) nextId = 0;
	newStage->init(tileMapsInfo[nextId].id, tileMapsInfo[nextId].mapName, currentStage->getSimon());
	setStage(newStage);
}

void StageManager::loadTileMaps()
{
	for (auto&& map : tileMapsInfo)
	{
		TileMapManager::getInstance()->add(map);
	}
}

void StageManager::reset(int id)
{
	auto newStage = new Stage();
	// cause map id from =1 so next one id from tileMapsInfo is this
	auto nextId = id == -1 ? currentStage->getId() : id;
	if (nextId > tileMapsInfo.size() - 1) nextId = 0;
	newStage->init(tileMapsInfo[nextId].id, tileMapsInfo[nextId].mapName);
	setStage(newStage);
}

void StageManager::onKeyDown(int keyCode)
{
	getCurrentStage()->onKeyDown(keyCode);
	switch (keyCode)
	{
	case DIK_Q: nextStage(0); break;
	case DIK_W: nextStage(1); break;
	case DIK_E: nextStage(2); break;
	case DIK_N: nextStage(); break;
	case DIK_R: reset(getCurrentStage()->getId() - 1); break;

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
	DebugOut(L"descrease\n");
	auto simon = currentStage->getSimon();
	const auto result = simon->updateLife(-1);
	if (result) {
		currentStage->reset();
		simon->setHp(SIM_MAX_HP);
		HUD::getInstance()->setTime(0);
	}
	else
	{
		// TODO: add end game screen
		simon->ForceDead();
	}
}