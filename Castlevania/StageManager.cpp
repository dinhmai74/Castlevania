#include "StageManager.h"
#include "HUD.h"
#include "Bubbles.h"
#include "IntroScene.h"
#include "ForceIdleSim.h"
#include "stage1_castle.h"
#include "Water.h"
#include "BrokenWall.h"
#include "BoundaryFactory.h"
#include "EnemyFactory.h"
#include "EnemyWolf.h"
#include "EnemyFish.h"
HUD* hud = HUD::getInstance();


StageManager* StageManager::instance = nullptr;

StageManager::~StageManager() {
	isReleaseSelectMapKey = true;
}


/*----------------- init  -----------------*/

void StageManager::init(vector<TileMapInfo> tileMapsInfo, vector<GridInfo> gridsInfo) {
	this->tileMapsInfo = tileMapsInfo;
	this->gridsInfo = gridsInfo;
	score = 0;

	endGameState = EndGameNone;
	isStartPlaying = 0;
	loadTileMaps();
	loadGrids();
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

void StageManager::loadGrids()
{
	for (auto grid : gridsInfo)
	{
		grids[grid.gridObjsName] = new Grid(grid.gridWidth, grid.gridHeight, grid.cellWidth, grid.cellHeight);
		loadObjectsFromFile(grid.gridObjsName);
	}
}

void StageManager::loadObjectsFromFile(wstring mapObjectsName)
{
	fstream infile;
	const wstring objectsPath = STAGE_PREFIX_PATH + mapObjectsName + STAGE_OBJECTS_PATH;
	infile.open(objectsPath.c_str(), ios::in);
	if (infile.fail()) {
		DebugOut(L"[ERROR] Scene %d load data failed: file path = %s\n", 1, objectsPath.c_str());
		infile.close();
		return;
	}

	int id;
	float x, y;
	std::string line;

	// read all file
	while (getline(infile, line)) {
		// read line
		std::istringstream fs(line);
		if (!(fs >> id >> x >> y)) continue;
		switch (id) {
		case 666: {
			int mapId;
			string mapObjectsName;
			fs >> mapId >> mapObjectsName;
			std::wstring wsTemp(mapObjectsName.begin(), mapObjectsName.end());
			StageManager::getInstance()->setCheckPoint({ mapId, wsTemp, x, y });
			break;
		}
		case OBBoundary: {
			float width, height;
			int type;
			fs >> width >> height >> type;
			auto boundary = BoundaryFactory::getInstance()->getBoundary(type);
			boundary->setWidhtHeight(width, height);
			boundary->setPos(x, y);
			boundary->setInitPos({ x, y });
			switch (type) {
			case BoundaryStair: {
				float stairType, faceSide, nextX, nextY, initPos;
				fs >> stairType >> faceSide >> nextX >> nextY;
				auto stair = dynamic_cast<Stair*>(boundary);
				if (stair) {
					if (stairType == StairStartDown || stairType == StairStartUp) {
						fs >> initPos;
						stair->setInitStairPos(initPos);
					}
					stair->setFaceSide(faceSide);
					stair->setStairType(stairType);
					stair->setNextPos({ nextX, nextY });
					add(stair, x, y, mapObjectsName);
				}
				break;
			}
			}
			break;
		}
		case OBItem: {
			int type;
			fs >> type;
			add(ItemFactory::Get()->getItem(type, { x, y }), x, y, mapObjectsName);
			break;
		}
		case OBCandle: {
			int type, itemContainType, itemNx;
			fs >> type >> itemContainType >> itemNx;
			add(CandleFactory::Get()->getCandle(type, itemContainType, itemNx, { x, y }), x, y, mapObjectsName);
			break;
		}

		case OBChangeStage: {
			float width, height, xPoint, yPoint, vx, vy, animId;
			int nextStageId;
			string nextStageName;
			fs >> width >> height >> nextStageId >> nextStageName >> xPoint >> yPoint >> vx >> vy >> animId;
			auto obj = new ObjectChangeStage();
			obj->setWidthHeight(width, height);
			obj->setPos(x, y);
			obj->setInitPos({ x, y });
			obj->setNextStageId(nextStageId);
			std::wstring wsTemp(nextStageName.begin(), nextStageName.end());
			obj->setNextStageMapObjName(wsTemp);
			obj->setChangeStateDestinationPoint({ xPoint, yPoint });
			obj->setChangeStateVelocity({ vx, vy });
			obj->setChangeStateAnimId(animId);
			add(obj, x, y, mapObjectsName);
			break;
		}

		case OBEnemy: {
			int type, faceSide, respawnTime, initState;
			float minRespawn, maxRespawn;
			fs >> faceSide >> type >> minRespawn >> maxRespawn >> respawnTime >> initState;
			auto obj = EnemyFactory::getInstance()->getEnemy(type);
			if (type == EnemWolf) {
				float activeTerLeft, activeTerRight;
				fs >> activeTerLeft >> activeTerRight;
				auto wolf = dynamic_cast<EnemyWolf*>(obj);
				wolf->setActiveTerritory({ activeTerLeft, activeTerRight });
			}
			else if (type == EnemFish) {
				float jumpRange, activeRange;
				fs >> jumpRange >> activeRange;
				auto fish = dynamic_cast<EnemyFish*>(obj);
				fish->setJumpingMaxRange(jumpRange);
				fish->setActiveRange(activeRange);
			}
			obj->setInitState(initState);
			obj->setState(initState);
			obj->setInitAnimId(initState);
			obj->setAnimId(initState);
			obj->setRespawnArea({ minRespawn, maxRespawn });
			obj->setInitPos({ x, y });
			obj->setPos(x, y);
			obj->setInitFaceSide(faceSide);
			obj->setFaceSide(faceSide);
			obj->setRespawnTime(respawnTime);
			obj->setEnable();
			add(obj, x, y, mapObjectsName);
			break;
		}
		case OBDoor: {
			int nx;
			float min, max, moveCam, newCheckPointX, newCheckPointY;
			int mapId;
			string name;
			fs >> nx >> min >> max >> moveCam >> newCheckPointX >> newCheckPointY >> mapId >> name;
			std::wstring wsTemp(name.begin(), name.end());
			auto obj = new Door();
			obj->setInitPos({ x, y });
			obj->setPos(x, y);
			obj->setFaceSide(nx);
			obj->setNextCameraLimit({ min, max });
			obj->setMoveCamDistance(moveCam);
			obj->setNewCheckPoint({ mapId, wsTemp, newCheckPointX, newCheckPointY });
			add(obj, x, y, mapObjectsName);
			break;
		}
		case OBForceIdleSim: {
			float width, height, nextX, nextY;
			int direction;
			fs >> width >> height >> direction >> nextX >> nextY;
			auto obj = new ForceIdleSim();
			obj->setWidhtHeight(width, height);
			obj->setInitPos({ x, y });
			obj->setPos(x, y);
			obj->setDirection(direction);
			obj->setNextX(nextX);
			obj->setNextY(nextY);
			add(obj, x, y, mapObjectsName);
			break;
		}

		case OBCastle: {
			auto obj = new Stage1Castle();
			obj->setInitPos({ x, y });
			obj->setPos(x, y);
			add(obj, x, y, mapObjectsName);
			break;
		}

		case OBBrokenWall: {
			float width, height;
			int itemId;
			fs >> width >> height >> itemId;
			auto obj = new BrokenWall(x, y);
			obj->setWidhtHeight(width, height);
			obj->setItemId(itemId);
			add(obj, x, y, mapObjectsName);
			break;
		}
		case OBBoss: {
			auto obj = new EnemyVampireBoss();
			obj->setInitPos({ x, y });
			obj->setPos(x, y);
			obj->setEnable();
			add(obj, x, y, mapObjectsName);
			break;
		}
		default:
			break;
		}
	}

	infile.close();
}

void StageManager::nextStage(int id, wstring mapNameInput) {
	if (id == -2) return;
	// cause map id from =1 so next one id from tileMapsInfo is this
	auto nextId = id == -1 ? currentStage->getId() : id;
	if (nextId > tileMapsInfo.size() - 1) nextId = 0;
	auto mapObjectsName = mapNameInput == L"none" ? tileMapsInfo[nextId].mapObjectsName : mapNameInput;
	currentStage->getSimon()->reset();
	currentStage->init(tileMapsInfo[nextId].id, mapObjectsName, currentStage->getSimon());
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

/*----------------- render update  -----------------*/
void StageManager::render() {
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

	if (isStartPlaying == 1) {
		if (!isGameOver) {
			if (!introDone) {
				resetStage(0, L"stage1");
				introDone = true;
			}
			getCurrentStage()->update(dt);
		}
		if (!currentStage->getIsGamePause() && endGameState == EndGameNone) { time += dt; }
		hud->update(dt);
	}
}

/*----------------- keyboard  -----------------*/

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
		case DIK_Q: nextStage(0, L"stage1"); break;
		case DIK_W: nextStage(1, L"stage2"); break;
		case DIK_E: nextStage(2, L"stage3"); break;
		case DIK_R: nextStage(2, L"stage3.1"); break;
		case DIK_T: nextStage(1, L"stage2.1"); break;
		case DIK_Y: nextStage(1, L"stage2.2"); break;
		case DIK_P: nextStage(1, L"stage2.3"); break;
		case DIK_O: {
			nextStage(1, L"stage2.3");
			currentStage->getSimon()->setPos({ 5150,0 });
			break;
		}
				  //case DIK_N: nextStage(); break;
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

/*----------------- utilities  -----------------*/
void StageManager::add(GameObject* ob) const {
	auto pos = ob->getPos();
	auto unit = new Unit(getCurrentStage()->getGrid(), ob, pos.x, pos.y);
}

void StageManager::add(GameObject* ob, float x, float y, wstring mapName) const
{
	auto grid = getGrids()[mapName];
	auto unit = new Unit(grid, ob, x, y);
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

bool StageManager::getPlaying()
{
	return isStartPlaying;
}

void StageManager::resetGame() {
	init(this->tileMapsInfo, this->gridsInfo);
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

