#include "Stage.h"
#include "ItemFactory.h"
#include "BoundaryFactory.h"
#include "SubWeaponHolyWater.h"
#include "ObjectChangeStage.h"
#include "EnemyFactory.h"
#include "ObjectFactory.h"
#include "Door.h"
#include "ForceIdleSim.h"
#include "stage1_castle.h"
#include "EnemyWolf.h"
#include "EnemyFish.h"
#include "EnemVampireBoss.h"
#include "Bubble.h"
#include "Water.h"
#include "StageManager.h"
#include "BrokenWall.h"
#include "HUD.h"

Stage::Stage() {
}

Stage::~Stage()
= default;

void Stage::init(int mapId, wstring mapName) {
	game->getCamera()->reset();
	initMap(mapId, mapName);
	initSimon();
	loadContent();
	timerStopEnemy->stop();
}

void Stage::init(int mapId, wstring mapName, Simon * simon) {
	this->renderBoundingBox = false;
	game->getCamera()->reset();
	this->simon = simon;
	this->simon->reset();
	initMap(mapId, mapName);
	loadContent();
}

void Stage::initMap(int mapId, wstring mapName) {
	game = Game::getInstance();
	this->mapId = mapId;
	this->mapName = std::move(mapName);
	const auto map = TileMapManager::getInstance()->get(mapId);
	this->grid = new Grid(map->getMapWidth(), 480, map->getTileWidth() * 4);
	game->setLimitCamX({ 0, static_cast<float>(map->getMapWidth()) });
}

void Stage::initSimon() {
	simon = new Simon();
	simon->setLife(3);
	simon->setEnergy(5);
	simon->setHp(SIM_MAX_HP);
}

void Stage::reset() {
	resetAllList();
	grid->reset();
	loadContent();
	simon->reset();
	simon->doUntouchable();
	game->setCameraPosition(initCam.x, initCam.y);
}

void Stage::loadContent() {
	loadObjectFromFiles();
}

void Stage::loadObjectFromFiles() {
	fstream fs;
	const wstring objectsPath = STAGE_PREFIX_PATH + mapName + STAGE_OBJECTS_PATH;
	fs.open(objectsPath.c_str(), ios::in);
	if (fs.fail()) {
		DebugOut(L"[ERROR] Scene %d load data failed: file path = %s\n", mapId, objectsPath.c_str());
		fs.close();
		return;
	}

	int id;
	float x, y;

	// read all file
	while (!fs.eof()) {
		// read line
		fs >> id >> x >> y;
		switch (id) {
		case 666:
		{
			int mapId;
			string mapObjectsName;
			fs >> mapId >> mapObjectsName;
			std::wstring wsTemp(mapObjectsName.begin(), mapObjectsName.end());
			StageManager::getInstance()->setCheckPoint({ mapId, wsTemp, x, y });
			break;
		}
		case OBSimon:
		{
			float max, min, camX, camY, climbDistance = 0;
			int nx, state, climbDirection = 1;
			fs >> nx >> min >> max >> camX >> camY >> state;
			if (state == climbing) {
				fs >> climbDistance >> climbDirection;
				simon->setStairDxRemain(climbDistance);
				simon->setStairDyRemain(climbDistance);
				simon->setClimbDirection(climbDirection);
			}
			else if (state == walking) {
				float autoWalkDistance, vAutoWalk;
				fs >> autoWalkDistance >> vAutoWalk;
				simon->doAutoWalkWithDistance(autoWalkDistance, vAutoWalk);
			}
			simon->setFaceSide(nx);
			simon->setPos(x, y);
			simon->setState(state);
			simon->setInitState(state);
			simon->setInitPos({ x, y });
			game->setLimitCamX({ min, max });
			initCam = { camX, camY };
			game->setCameraPosition(camX, camY);
			DebugOut(L"\n load simon");
			break;
		}
		case OBBoundary:
		{
			loadBoundaryCase(fs, x, y);
			break;
		}
		case OBItem:
		{
			int type;
			fs >> type;
			auto item = ItemFactory::Get()->getItem(type, { x, y });
			auto unit = new Unit(getGrid(), item, x, y);
			DebugOut(L"\n load item");
			break;
		}
		case OBCandle:
		{
			int type, itemContainType, itemNx;
			fs >> type >> itemContainType >> itemNx;
			const auto candle = CandleFactory::Get()->getCandle(type, itemContainType, itemNx, { x, y });
			auto unit = new Unit(getGrid(), candle, x, y);
			DebugOut(L"\n load candle");
			break;
		}

		case OBChangeStage:
		{
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
			auto unit = new Unit(getGrid(), obj, x, y);
			DebugOut(L"\n load obChangeStage");
			break;
		}

		case OBEnemy:
		{
			loadEnemies(fs, x, y);
			break;
		}
		case OBDoor:
		{
			int nx;
			float min, max, moveCam, newCheckPointX, newCheckPointY;
			int mapId;
			string mapObjectsName;
			fs >> nx >> min >> max >> moveCam >> newCheckPointX >> newCheckPointY >> mapId >> mapObjectsName;
			std::wstring wsTemp(mapObjectsName.begin(), mapObjectsName.end());
			auto obj = new Door();
			obj->setInitPos({ x, y });
			obj->setPos(x, y);
			obj->setFaceSide(nx);
			obj->setNextCameraLimit({ min, max });
			obj->setMoveCamDistance(moveCam);
			obj->setNewCheckPoint({ mapId, wsTemp, newCheckPointX, newCheckPointY });
			auto unit = new Unit(getGrid(), obj, x, y);
			break;
		}
		case OBForceIdleSim:
		{
			float width, height, nextX, nextY;
			int direction;
			fs >> width >> height >> direction >> nextX >> nextY;
			auto ob = new ForceIdleSim();
			ob->setWidhtHeight(width, height);
			ob->setInitPos({ x, y });
			ob->setPos(x, y);
			ob->setDirection(direction);
			ob->setNextX(nextX);
			ob->setNextY(nextY);
			auto unit = new Unit(getGrid(), ob, x, y);
			break;
		}
		case OBCastle:
		{
			auto ob = new Stage1Castle();
			ob->setInitPos({ x, y });
			ob->setPos(x, y);
			DebugOut(L"load castle\n");
			auto unit = new Unit(getGrid(), ob, x, y);
			break;
		}
		case OBBoss:
		{
			setBoss(new EnemyVampireBoss());
			boss->setInitPos({ x, y });
			boss->setPos(x, y);
			boss->setEnable();
			DebugOut(L"load boss\n");
			auto unit = new Unit(grid, boss, x, y);
			break;
		}

		case OBWater:
		{
			float width, height;
			fs >> width >> height;
			auto obj = new Water();
			obj->setPos(x, y);
			obj->setInitPos({ x, y });
			obj->setWidhtHeight(width, height);
			listWater.push_back(obj);
			break;
		}
		case OBBrokenWall: {

			float width, height;
			int itemId;
			fs >> width >> height>> itemId;
			auto obj = new BrokenWall(x,y);
			obj->setWidhtHeight(width, height);
			obj->setItemId(itemId);
			auto unit = new Unit(grid, obj, x, y);
		}
		default:
			break;
		}
	}

	fs.close();
}

void Stage::loadBoundaryCase(fstream& fs, float x, float y) {
	float width, height;
	int type;
	fs >> width >> height >> type;
	auto boundary = BoundaryFactory::getInstance()->getBoundary(type);
	boundary->setWidhtHeight(width, height);
	boundary->setPos(x, y);
	boundary->setInitPos({ x, y });
	switch (type) {
	case BoundaryStair:
	{
		float stairType, faceSide, nextX, nextY;
		fs >> stairType >> faceSide >> nextX >> nextY;
		auto stair = dynamic_cast<Stair*>(boundary);
		if (stair) {
			stair->setFaceSide(faceSide);
			stair->setStairType(stairType);
			stair->setNextPos({ nextX, nextY });
			auto unit = new Unit(grid, boundary, x, y);
			listStairs.push_back(boundary);
		}
		break;
	}
	case BoundaryNormal:
	case BoundaryGround:
	case BoundaryTwoSide:
		listCanCollideBoundary.push_back(boundary);
		break;
	default:
		auto unit = new Unit(grid, boundary, x, y);
		listDefaultBoundary.push_back(boundary);
	}
}

void Stage::loadEnemies(fstream& fs, float x, float y) {
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
		fs >> jumpRange>> activeRange;
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
	auto unit = new Unit(getGrid(), obj, x, y);
}

void Stage::render() {
	TileMapManager::getInstance()->get(mapId)->draw();

	for (auto object : listRenderObj) {
		if (renderBoundingBox) object->renderBoundingBox();
		object->render();
	}
}

bool Stage::updateEnemy(vector<GameObject*>::value_type obj, DWORD dt) {
	auto enem = dynamic_cast<Enemy*>(obj);
	if (enem) {
		if (simon->isWalkingOutDoor() || isStopEnemyForDebug) {
			enem->reset();
			return true;
		}

		enem->setIsStopAllAction(timerStopEnemy->isRunning()|| isGamePause);

		if (isGamePause || timerStopEnemy->isRunning()) return true;
		vector<GameObject*> canColide;
		canColide = listCanCollideBoundary;
		canColide.insert(canColide.begin(), listDefaultBoundary.begin(), listDefaultBoundary.end());
		canColide.insert(canColide.begin(), listWater.begin(), listWater.end());

		auto wolf = dynamic_cast<EnemyWolf*>(obj);
		if (wolf)
			wolf->update(dt, &canColide, simon->getPos().x);
		else
			enem->update(dt, &canColide);

		return true;
	}
	return false;
}

void Stage::update(DWORD dt) {
	loadListObjFromGrid();

	const auto map = getMapSimonCanCollisionObjects();
	respawnEnemies();

	simon->setIsStopAllAction(isGamePause);
	simon->update(dt, map);
	for (auto obj : listRenderObj) {
		if (obj->getType() == OBSimon) continue;

		obj->setIsStopAllAction(isGamePause);
		auto subWeapon = dynamic_cast<SubWeapon*>(obj);
		if (subWeapon) {
			if(!isGamePause) updateSubWeapon(subWeapon, dt);
			continue;
		}


		auto updateResult = updateEnemy(obj, dt);

		auto listCO = listCanCollideBoundary;
		listCO.insert(listCO.begin(), listWater.begin(), listWater.end());
		if (!updateResult && !isGamePause) obj->update(dt, &listCanCollideBoundary);
	}

	updateCamera(dt);
	updateGrid();
	updateInActiveUnit();
}

void Stage::updateSubWeapon(SubWeapon* subWeapon, DWORD dt) {
	auto simonPos = getSimon()->getPos();
	auto holyWater = dynamic_cast<SubWeaponHolyWater*>(subWeapon);
	vector<GameObject*> temp = listCanHitObjects;
	temp.push_back(simon);
	temp.insert(temp.end(), listEnemy.begin(), listEnemy.end());
	if (holyWater) {
		temp.insert(temp.end(), listCanHitObjects.begin(), listCanHitObjects.end());
		vector<MapGameObjects> maps;
		maps.push_back({ OBEnemy, &temp });
		maps.push_back({ OBBoundary, &listCanCollideBoundary });

		holyWater->update(dt, simonPos, simon->getState(), maps);
	}
	else {
		subWeapon->update(dt, simonPos, simon->getState(), &temp);
	}
}

void Stage::respawnEnemies() {
	if (isFightingBoss) return;
	for (auto obj : listEnemy) {
		float playerX, playerY;
		simon->getPos(playerX, playerY);
		auto enemy = dynamic_cast<Enemy*>(obj);
		if (enemy) enemy->respawn(playerX, playerY);
	}
}

vector<MapGameObjects> Stage::getMapSimonCanCollisionObjects() {
	vector<MapGameObjects> map;
	map.push_back({ OBForceIdleSim, &listForceIdleSim });
	map.push_back({ OBStair, &listStairs });
	map.push_back({ OBItem, &listItems });
	map.push_back({ OBCanHitObjs, &listCanHitObjects });
	map.push_back({ OBChangeStage, &listObjectChangeStage });
	map.push_back({ OBEnemy, &listEnemy });
	map.push_back({ OBBoundary, &listStopSimObjs });
	map.push_back({ OBBullet, &listBullet });
	map.push_back({ OBWater, &listWater });
	return map;
}

void Stage::updateInActiveUnit() {
	for (auto ob : listRenderObj) {
		if (!isInViewport(ob)) {
			auto type = ob->getType();
			switch (type) {
			case OBSubWeapon:
				ob->setActive(false);
				ob->setEnable(false);
				break;
			default:;
			}
		}
	}

	for (auto ob : listEnemy) {
		auto enemy = dynamic_cast<Enemy*>(ob);
		if (enemy && !isInViewport(enemy)) {
			// set respawn immediately when user go out the init enemy pos
			auto initPos = enemy->getInitPos();
			auto enemyBBox = enemy->getBoundingBoxBaseOnFile();
			Box box;
			box.l = initPos.x;
			box.t = initPos.y;
			box.r = box.l + (enemyBBox.r - enemyBBox.l);
			box.b = box.t + (enemyBBox.b - enemyBBox.t);

			if (!isInViewPort(box) && !enemy->getReadyToRespawn()) {
				enemy->setRespawnTime(1);
				enemy->reset();
				enemy->setEnable(false);
				continue;
			}

			// start timer when enemy ran out the view port
			if (!enemy->getReadyToRespawn() && enemy->IsEnable()) {
				enemy->reset();
				enemy->setEnable(false);
			}
		}
	}
}

bool Stage::isInViewport(GameObject* object) {
	return isInViewPort(object->getBoundingBox());
}



bool Stage::isInViewPort(Box pos) {
	const auto camPosition = Game::getInstance()->getCameraPosition();
	const auto left = camPosition.x;
	const auto right = camPosition.x + SCREEN_WIDTH;
	const auto isInView = isColliding(pos,
		{
			left, camPosition.y, right,
			camPosition.y + SCREEN_HEIGHT
		});


	return isInView;
}


void Stage::updateGrid() {
	for (auto unit : listUnit) {
		auto obj = unit->get();
		if (!obj->IsEnable() || !obj->IsActive()) continue;

		const auto pos = obj->getPos();
		unit->move(pos.x, pos.y);
	}
}

bool sortByType(GameObject* a, GameObject* b) {
	return a->getType() < b->getType();
}

void Stage::loadListObjFromGrid() {
	resetAllUnitList();
	listRenderObj = listCanCollideBoundary;
	listStopSimObjs = listCanCollideBoundary;
	listRenderObj.push_back(simon);
	listRenderObj.insert(listRenderObj.begin(), listWater.begin(), listWater.end());
	getGrid()->get(Game::getInstance()->getCameraPosition(), listUnit);

	for (auto unit : listUnit) {
		auto obj = unit->get();
		listRenderObj.push_back(obj);

		const auto type = obj->getType();
		switch (type) {
		case OBItem: listItems.push_back(obj);
			break;
		case OBChangeStage: listObjectChangeStage.push_back(obj);
			break;
		case OBCandle: listCanHitObjects.push_back(obj);
			break;
		case OBEnemy: listEnemy.push_back(obj);
			break;
		case OBDoor: listDoor.push_back(obj);
			listStopSimObjs.push_back(obj);
			break;
		case OBForceIdleSim: listForceIdleSim.push_back((obj));
			break;
		case OBBrokenWall:
			listCanHitObjects.push_back(obj);
			listStopSimObjs.push_back(obj);
			break;
		case OBBullet:
			listBullet.push_back(obj);
			break;
		default:;
		}
	}

	if (boss) {
		listEnemy.push_back(boss);
		listCanHitObjects.push_back(boss);
	}
	sort(listRenderObj.begin(), listRenderObj.end(), sortByType);
}

void Stage::resetAllList() {
	resetAllUnitList();
	listDefaultBoundary.clear();
	listCanCollideBoundary.clear();
	listStairs.clear();
	listWater.clear();
}


void Stage::clearMapByItem() {
	for (auto enemy : listEnemy)
		enemy->getHurt(-9999999);
}

void Stage::resetAllUnitList() {
	listUnit.clear();
	listRenderObj.clear();
	listItems.clear();
	listCanHitObjects.clear();
	listEnemy.clear();
	listDoor.clear();
	listStopSimObjs.clear();
	listObjectChangeStage.clear();
	listForceIdleSim.clear();
	listBullet.clear();
}

void Stage::updateCamera(const DWORD dt) const {
	if (game->getCamera()->getIsLocked()) return;
	auto res = game->getCamera()->update(dt);
	if (res) return;
	float simonX, simonY, simonVx, simonVy;
	getSimon()->getPos(simonX, simonY);
	getSimon()->getSpeed(simonVx, simonVy);

	const int offset = 60;

	float posX, posY;
	game->getCameraPosition(posX, posY);
	auto newX = posX;
	auto newY = posY;

	const auto isCanMoveAreaX =
		simonX + offset > SCREEN_WIDTH / 2 + game->getLimitCamX().min &&
		simonX + offset - 20 + SCREEN_WIDTH / 2 < game->getLimitCamX().max;

	if (isCanMoveAreaX)
		newX = simonX + offset - SCREEN_WIDTH / 2;

	/*if (simonY + offset > SCREEN_HEIGHT / 2 &&
		simonY + offset + SCREEN_HEIGHT / 2 < mapHeight){
		posY = simonY + offset - SCREEN_HEIGHT / 2 - HEADER_HEIGHT;
	}*/
	game->setCameraPosition(newX, newY);
}

void Stage::onKeyDown(const int keyCode) {
	if (!isGamePause) getSimon()->handleOnKeyDown(keyCode);
	switch (keyCode) {
	case DIK_B: renderBoundingBox = !renderBoundingBox;
		break;
	case DIK_U: simon->powerUpWhip();
		break;
	case DIK_D: simon->powerUpWhip(false);
		break;
	case DIK_X: simon->getHurt(1, 1, 1);
		break;
	case DIK_C: simon->getHurt(1, 1, 666);
		break;
	case DIK_0: simon->setSubWeapon(itemDagger);
		break;
	case DIK_1: simon->setSubWeapon(itemAxe);
		break;
	case DIK_2: simon->setSubWeapon(itemBoomerang);
		break;
	case DIK_3: simon->setSubWeapon(itemHolyWater);
		break;
	case DIK_4: simon->setSubWeapon(itemStopWatch);
		break;
	case DIK_F: simon->addEnergy(100);
		break;
	case DIK_G: isStopEnemyForDebug = !isStopEnemyForDebug;
		break;
	case DIK_PAUSE:
		isGamePause = !isGamePause;
		HUD::getInstance()->setIsGamePause(isGamePause);
		break;
	default:;
	}
}

void Stage::onKeyUp(const int keyCode) const {
	getSimon()->handleOnKeyRelease(keyCode);
}

void Stage::keyState(BYTE* states) const {
	if (!isGamePause) getSimon()->handleOnKeyPress(states);
}

Grid* Stage::getGrid() const {
	return grid;
}

Simon* Stage::getSimon() const {
	return simon;
}

int Stage::getId() {
	return mapId;
}

void Stage::stopEnemyForABit(DWORD time) {
	if(!timerStopEnemy->isRunning()) {
		timerStopEnemy = new Timer(time);
		timerStopEnemy->start();
	}
}
