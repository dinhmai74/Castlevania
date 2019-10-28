#include "StageManager.h"
#include "HUD.h"
#include "Bubbles.h"
HUD* hud = HUD::getInstance();


StageManager* StageManager::instance = nullptr;

void StageManager::pauseGame(bool val) const {
	currentStage->setGamePause(val);
}

void StageManager::removeAllObjOutOfBound() {
	currentStage->getGrid()->removeOutOfBoundUnit(Camera::getInstance()->getLimitX());
}

StageManager::~StageManager() {
	isReleaseSelectMapKey = true;
}

void StageManager::init(vector<TileMapInfo> tileMapsInfo) {
	this->tileMapsInfo = tileMapsInfo;
	score = 0;

	loadTileMaps();
	currentStage = new Stage();
	currentStage->init(tileMapsInfo[0].id, tileMapsInfo[0].mapName);

	hud->init();
}

void StageManager::nextStage(int id, wstring mapNameInput) {
	if (id == -2) return;
	auto newStage = new Stage();
	// cause map id from =1 so next one id from tileMapsInfo is this
	auto nextId = id == -1 ? currentStage->getId() : id;
	if (nextId > tileMapsInfo.size() - 1) nextId = 0;
	auto mapName = mapNameInput == L"none" ? tileMapsInfo[nextId].mapName : mapNameInput;
	newStage->init(tileMapsInfo[nextId].id, mapName, currentStage->getSimon());
	newStage->getSimon()->resetState();
	setStage(newStage);
}

void StageManager::loadTileMaps() {
	for (auto&& map : tileMapsInfo) {
		TileMapManager::getInstance()->add(map);
	}
}

void StageManager::reset(int id) {
	auto newStage = new Stage();
	// cause map id from =1 so next one id from tileMapsInfo is this
	auto nextId = id == -1 ? currentStage->getId() : id;
	if (nextId > tileMapsInfo.size() - 1) nextId = 0;
	newStage->init(tileMapsInfo[nextId].id, tileMapsInfo[nextId].mapName);
	setStage(newStage);
}

void StageManager::render() const {
	getCurrentStage()->render();
	hud->render();
}

void StageManager::update(const DWORD dt) const {
	getCurrentStage()->update(dt);
	hud->update(dt, currentStage->getIsGamePause());
}

void StageManager::onKeyDown(int keyCode) {
	getCurrentStage()->onKeyDown(keyCode);
	switch (keyCode) {
	case DIK_Q: nextStage(0); break;
	case DIK_W: nextStage(1); break;
	case DIK_E: nextStage(2); break;
	case DIK_R: nextStage(2, L"stage3.1"); break;
	case DIK_T: nextStage(1, L"stage2.1"); break;
	case DIK_Y: nextStage(1, L"stage2.2"); break;
	case DIK_P: nextStage(1, L"stage2.3"); break;
	case DIK_N: nextStage(); break;
	case DIK_H:
	{
		auto bubles = new Bubbles(20,100);
		add(bubles);

		break;
	}

	default:
		break;
	}
}

void StageManager::onKeyUp(int keyCode) {
	getCurrentStage()->onKeyUp(keyCode);
}

void StageManager::keyState(BYTE* states) {
	getCurrentStage()->keyState(states);
}

void StageManager::add(GameObject* ob) const {
	auto pos = ob->getPosition();
	auto unit = new Unit(getCurrentStage()->getGrid(), ob, pos.x, pos.y);
}

void StageManager::descreaseLife() {
	DebugOut(L"descrease\n");
	auto simon = currentStage->getSimon();
	const auto result = simon->updateLife(-1);
	if (result) {
		currentStage->reset();
		simon->setHp(SIM_MAX_HP);
		HUD::getInstance()->setTime(0);
	}
	else {
		// TODO: add end game screen
	}
}