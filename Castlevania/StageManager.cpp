#include "StageManager.h"
#include "HUD.h"
#include "Bubbles.h"
#include "IntroScene.h"
HUD* hud = HUD::getInstance();


StageManager* StageManager::instance = nullptr;

void StageManager::resetGame() {
	init(this->tileMapsInfo);
	isStartPlaying = 1;
	endGameState = EndGameNone;
}

bool StageManager::getIsWhipMaxLv() {
	return currentStage->getSimon()->getWhip()->isMaxLv();
}

void StageManager::pauseGame(bool val) const {
	currentStage->setGamePause(val);
}

void StageManager::clearMapByItem() {
	currentStage->clearMapByItem();
}

void StageManager::stopEnemyForABit(DWORD time) {
	currentStage->stopEnemyForABit(time);
}

void StageManager::removeAllObjOutOfBound() {
	currentStage->getGrid()->removeOutOfBoundUnit(Camera::getInstance()->getLimitX());
}

void StageManager::addSubWeapon(SubWeapon* subWeapon) {
	currentStage->addSubWeapon(subWeapon);
}

void StageManager::setEndGame() {
	if (endGameState == EndGameNone) {
		endGameState = EndGameStart;
		timerEndGame->start();
	}
}

StageManager::~StageManager() {
	isReleaseSelectMapKey = true;
}

void StageManager::init(vector<TileMapInfo> tileMapsInfo) {
	this->tileMapsInfo = tileMapsInfo;
	score = 0;

	endGameState = EndGameNone;
	isStartPlaying = 0;
	loadTileMaps();
	currentStage = new Stage();
	currentStage->init(tileMapsInfo[0].id, tileMapsInfo[0].mapObjectsName);
	checkPoint = { 0,L"stage1",0,305 };
	playerChoseWhenOver = 0;
	isGameOver = false;
	IntroScene::getInstance()->init();
	defaultTime = DEFAULT_TIME_PLAY;
	time = 0;

	hud->init();
}

void StageManager::nextStage(int id, wstring mapNameInput) {
	if (id == -2) return;
	auto newStage = new Stage();
	// cause map id from =1 so next one id from tileMapsInfo is this
	auto nextId = id == -1 ? currentStage->getId() : id;
	if (nextId > tileMapsInfo.size() - 1) nextId = 0;
	auto mapObjectsName = mapNameInput == L"none" ? tileMapsInfo[nextId].mapObjectsName : mapNameInput;
	currentStage->getSimon()->reset();
	newStage->init(tileMapsInfo[nextId].id, mapObjectsName, currentStage->getSimon());
	setStage(newStage);
}

void StageManager::loadTileMaps() {
	for (auto&& map : tileMapsInfo) {
		TileMapManager::getInstance()->add(map);
	}
}

void StageManager::resetStage(int id, wstring mapName) {
	auto newStage = new Stage();
	// cause map id from =1 so next one id from tileMapsInfo is this
	auto nextId = id == -1 ? currentStage->getId() : id;
	if (nextId > tileMapsInfo.size() - 1) nextId = 0;
	newStage->init(tileMapsInfo[nextId].id, mapName, currentStage->getSimon());
	setStage(newStage);
	time = 0;
}

void StageManager::render() const {
	if (endGameState == EndGameDone) return;
	if (isStartPlaying == ID_MAIN_MENU) {
		IntroScene::getInstance()->render();
	}
	else {
		if (!isGameOver) getCurrentStage()->render();
		hud->render();
	}
}

void StageManager::updateEndGame() {
	auto sim = currentStage->getSimon();
	switch (endGameState) {
	case EndGameStart: {
		sim->setState(idle);
		if (timerEndGame->isTimeUpAndRunAlr()) {
			sim->addHP(SIM_MAX_HP);
			endGameState = EndGameAddHp;
		}
		break;
	}
	case EndGameAddHp:
		if (sim->getHp() >= SIM_MAX_HP) endGameState = EndGameTimeToScore;
		break;
	case EndGameTimeToScore:
		time += dt * 100;
		score += dt;
		if (getRemainTime() <= 0) endGameState = EndGameHeartToScore;
		break;
	case EndGameHeartToScore: {
		auto heart = sim->getEnergy();
		if (heart > 0) {
			if (timerCountHeart->isTimeUp()) {
				sim->loseEnergy(1);
				score += 100;
				timerCountHeart->start();
			}
		}
		else {
			endGameState = EndGameDone;
		}

		break;
	}
	default:;
	}
}

void StageManager::update(const DWORD dt) {
	if (endGameState == EndGameDone) return;
	this->dt = dt;
	updateEndGame();
	if (isStartPlaying == ID_MAIN_MENU) {
		IntroScene::getInstance()->update(dt);
	}
	else {
		if (!isGameOver)getCurrentStage()->update(dt);
		if (!currentStage->getIsGamePause() && endGameState == EndGameNone) { time += dt; }
		hud->update(dt);
	}
}

void StageManager::onKeyDown(int keyCode) {
	if (endGameState != EndGameNone || endGameState == EndGameDone) return;
	if (isGameOver) {
		if (keyCode == DIK_UP || keyCode == DIK_DOWN)
			playerChoseWhenOver = (playerChoseWhenOver + 1) % 2;
		else if (keyCode == 28) {
			if (playerChoseWhenOver == 0)		// Continue game
				resetGame();
			else {
				HWND hWnd = GetActiveWindow();		// End game
				DestroyWindow(hWnd);
			}
		}
	}
	else if (isStartPlaying == ID_MAIN_MENU) {
		auto intro = IntroScene::getInstance();
		if (intro->getIsReady()) {
			isStartPlaying = 1;
		}
		else { intro->setIsReady(true); }
	}
	else {
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
		case DIK_M: setEndGame(); break;
		default:
			break;
		}
	}
}

void StageManager::onKeyUp(int keyCode) {
	if (endGameState != EndGameNone || endGameState == EndGameDone) return;
	if (isGameOver || isStartPlaying == ID_MAIN_MENU) return;
	getCurrentStage()->onKeyUp(keyCode);
}

void StageManager::keyState(BYTE* states) {
	if (endGameState != EndGameNone || endGameState == EndGameDone) return;
	if (isGameOver)  return;
	getCurrentStage()->keyState(states);
}

void StageManager::add(GameObject* ob) const {
	auto pos = ob->getPos();
	auto unit = new Unit(getCurrentStage()->getGrid(), ob, pos.x, pos.y);
}

void StageManager::descreaseLife() {
	auto simon = currentStage->getSimon();
	const auto result = simon->addLife(-1);
	if (result) {
		resetStage(checkPoint.mapId, checkPoint.mapName);
		simon->reset();
		simon->setHp(SIM_MAX_HP);
		simon->setState(idle);
		simon->setFaceSide(SideRight);
		simon->doUntouchable();
		simon->setPos(checkPoint.x, checkPoint.y);
		simon->setInitPos({ checkPoint.x,checkPoint.y });
	}
	else {
		isGameOver = true;
	}
}