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

bool sortByType(GameObject* a, GameObject* b) {
	return a->getType() < b->getType();
}

Stage::Stage() {
	this->renderBoundingBox = false;
	mapName = L"stage1";
	simon = new Simon();
}

Stage::~Stage()
= default;

void Stage::init(int mapId, wstring mapName) {
	game->getCamera()->reset();
	initMap(mapId, mapName);
	initSimon();
	timerStopEnemy->stop();
}

void Stage::init(int mapId, wstring mapName, Simon * simon) {
	game->getCamera()->reset();
	this->simon = simon;
	this->simon->reset();
	initMap(mapId, mapName);
}

void Stage::initMap(int mapId, wstring mapName) {
	game = Game::getInstance();
	resetAllList();
	this->mapId = mapId;
	this->mapName = std::move(mapName);
	const auto map = TileMapManager::getInstance()->get(mapId);
	grid = StageManager::getInstance()->getGrids()[this->mapName];
	game->setLimitCamX({ 0, static_cast<float>(map->getMapWidth()) });
	loadContent();
}

void Stage::initSimon() {
	simon = new Simon();
	simon->setLife(3);
	simon->setEnergy(5);
	simon->setHp(SIM_MAX_HP);
}

void Stage::add(GameObject* ob, D3DXVECTOR2 initPos) {
	auto unit = new Unit(grid, ob, initPos.x, initPos.y);
}

void Stage::reset() {
	resetAllList();
	grid->reset();
	simon->reset();
	simon->doUntouchable();
	game->setCameraPosition(initCam.x, initCam.y);
}

void Stage::loadContent() {
	loadObjectFromFiles();
}

void Stage::loadObjectFromFiles() {
	const wstring objectsPath = STAGE_PREFIX_PATH + mapName + STAGE_OBJECTS_PATH;

	// const wstring objectsPath = L"stages\\stage1_objects.txt";

	fstream infile;
	infile.open(objectsPath.c_str(), ios::in);
	if (infile.fail()) {
		DebugOut(L"[ERROR] Scene %d load data failed: file path = %s\n", 1, objectsPath.c_str());
		infile.close();
		return;
	}

	int id;
	float x, y;
	if (infile.is_open()) {
		// read all file
		std::string line;
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
			case OBSimon: {
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
				case BoundaryGround:
					listGround.push_back(boundary);
				case BoundaryNormal:
				case BoundaryTwoSide:
					listCanCollideBoundary.push_back(boundary);
					break;
				default:
					listDefaultBoundary.push_back(boundary);
				}
				break;
			}
			case OBWater: {
				float width, height;
				fs >> width >> height;
				auto obj = new Water();
				obj->setPos(x, y);
				obj->setInitPos({ x, y });
				obj->setWidhtHeight(width, height);
				listWater.push_back(obj);
				break;
			}
			default:
				break;
			}
		}

		infile.close();
	}
}


void Stage::render() {
	TileMapManager::getInstance()->get(mapId)->draw();

	for (auto object : listRenderObj) {
		if (renderBoundingBox) object->renderBoundingBox();
		if (object->getType() == OBEnemy && isFightingBoss)  continue;
		object->render();
	}
}

bool Stage::updateEnemy(vector<GameObject*>::value_type obj, DWORD dt) {
	auto enem = dynamic_cast<Enemy*>(obj);
	if (enem) {
		if (isFightingBoss && enem->getType() == OBEnemy) return true;
		if (simon->isWalkingOutDoor() || isStopEnemyForDebug) {
			enem->reset();
			return true;
		}

		enem->setIsStopAllAction(timerStopEnemy->isRunning() || isGamePause);

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
			if (!isGamePause) updateSubWeapon(subWeapon, dt);
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
		maps.push_back({ OBBoundary, &listGround });

		holyWater->update(dt, simonPos, simon->getState(), maps);
	}
	else {
		subWeapon->update(dt, simonPos, simon->getState(), &temp);
	}
}

void Stage::respawnEnemies() {
	if (isFightingBoss || timerStopEnemy->isRunning()) return;
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
	map.push_back({ OBEndGame, &listObjectsEndGame });
	return map;
}

void Stage::updateInActiveUnit() {
	for (auto ob : listRenderObj) {
		if (!isInViewport(ob)) {
			auto type = ob->getType();
			switch (type) {
			case OBSubWeapon:
				ob->setDisable();
				removeSubWeapons(ob);
				break;
			case OBBullet:
				ob->setDisable();
				break;
			default:;
			}
		}
	}

	for (auto ob : listEnemy) {
		auto enemy = dynamic_cast<Enemy*>(ob);
		if (enemy && !isInViewport(enemy)) {
			// set respawn immediately when user go out the init enemy pos
			if (enemy->getType() == OBBoss) {
				boss->setState(sleep);
				continue;
			}
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

void Stage::loadListObjFromGrid() {
	resetAllUnitList();
	//listRenderObj = listCanCollideBoundary;
	listStopSimObjs = listCanCollideBoundary;
	listRenderObj.push_back(simon);
	listRenderObj.insert(listRenderObj.begin(), subWeapons.begin(), subWeapons.end());
	grid->get(Game::getInstance()->getCameraPosition(), listUnit);

	for (auto unit : listUnit) {
		auto obj = unit->get();

		const auto type = obj->getType();
		const auto notRenderObjs = type == OBBoundary || type == OBForceIdleSim || type == OBChangeStage;
		if (!notRenderObjs) listRenderObj.push_back(obj);
		switch (type) {
		case OBItem: listItems.push_back(obj);
			break;
		case OBChangeStage: listObjectChangeStage.push_back(obj);
			break;
		case OBCandle: listCanHitObjects.push_back(obj);
			break;
		case OBEnemy: if (!isFightingBoss) listEnemy.push_back(obj);
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
			listCanHitObjects.push_back(obj);
			listBullet.push_back(obj);
			break;
		case OBEndGame:
			listObjectsEndGame.push_back(obj);
			break;
		case OBBoundary:
		{
			auto boun = dynamic_cast<Boundary*>(obj);
			if (boun && boun->getBoundaryType() == BoundaryStair) listStairs.push_back(boun);
			break;
		}
		case OBBoss: {
			auto temp = dynamic_cast<EnemyVampireBoss*>(obj);
			boss = temp;
			listEnemy.push_back(temp);
			listCanHitObjects.push_back(temp);
			break;
		}
		default:break;
		}
	}

	sort(listRenderObj.begin(), listRenderObj.end(), sortByType);
}

void Stage::resetAllList() {
	resetAllUnitList();
	listDefaultBoundary.clear();
	listCanCollideBoundary.clear();
	listGround.clear();
	listStairs.clear();
	listWater.clear();
}

void Stage::clearMapByItem() {
	for (auto enemy : listEnemy)
	{
		if (enemy->getType() != OBBoss)
			enemy->getHurt(1, 9999999);
	}
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
	listStairs.clear();
	listObjectsEndGame.clear();
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

/*----------------- keyboard  -----------------*/

void Stage::onKeyDown(const int keyCode) {
	if (!isGamePause) getSimon()->handleOnKeyDown(keyCode);
	switch (keyCode) {
	case DIK_B: renderBoundingBox = !renderBoundingBox;
		break;
	case DIK_U: simon->powerUpWhip();
		break;
	case DIK_D: simon->powerUpWhip(false);
		break;
	case DIK_X: simon->getHurt(1,1,4);
		break;
	case DIK_C: simon->getHurt(1,1,666);
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
	case DIK_5: {
		debugItem(itemGoldPotion);
		break;
	}
	case DIK_6: {
		debugItem(itemDoubleShot);
		break;
	}
	case DIK_7: {
		debugItem(itemTripleShot);
		break;
	}
	case DIK_8: {
		debugItem(itemPorkChop);
		break;
	}
	case DIK_9: {
		debugItem(itemHolyCross);
		break;
	}
	case DIK_F: simon->addEnergy(100);
		break;
	case DIK_H: if (boss) boss->getHurt(1);
		break;
	case DIK_J: if (boss) boss->getHurt(1, 999);
		break;
	case DIK_L: if (boss) boss->resetPos();
		break;
		//case DIK_G: isStopEnemyForDebug = !isStopEnemyForDebug;
		//	break;

	case DIK_PAUSE:
		isGamePause = !isGamePause;
		HUD::getInstance()->setIsGamePause(isGamePause);
		break;
	default:;
	}
}

void Stage::debugItem(int itemId)
{
	auto ob = ItemFactory::Get()->getItem(itemId, { simon->getPos().x + 50,0 });
	add(ob, ob->getPos());
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
	if (!timerStopEnemy->isRunning()) {
		timerStopEnemy = new Timer(time);
		timerStopEnemy->start();
	}
}

void Stage::addSubWeapon(SubWeapon* subWeapon) {
	subWeapons.push_back(subWeapon);
}

void Stage::removeSubWeapons(GameObject* ob) {
	subWeapons.erase(std::remove(subWeapons.begin(), subWeapons.end(), ob), subWeapons.end());
}
