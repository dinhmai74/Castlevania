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

Stage::Stage()
{
}

Stage::~Stage()
= default;

void Stage::init(int mapId, wstring mapName)
{
	this->renderBoundingBox = false;
	game->getCamera()->reset();
	initMap(mapId, mapName);
	initSimon();
	loadContent();
}

void Stage::init(int mapId, wstring mapName, Simon * simon)
{
	this->renderBoundingBox = false;
	game->getCamera()->reset();
	this->simon = simon;
	this->simon->reset();
	initMap(mapId, mapName);
	loadContent();
}

void Stage::initMap(int mapId, wstring mapName)
{
	game = Game::getInstance();
	this->mapId = mapId;
	this->mapName = std::move(mapName);
	const auto map = TileMapManager::getInstance()->get(mapId);
	this->grid = new Grid(map->getMapWidth(), 480, map->getTileWidth() * 2);
	game->setLimitCamX({ 0, static_cast<float>(map->getMapWidth()) });
}

void Stage::initSimon()
{
	simon = new Simon();
	simon->doAutoWalk();
	simon->setLife(3);
	simon->setEnergy(5);
	simon->setHp(SIM_MAX_HP);
}

void Stage::reset()
{
	resetAllList();
	loadContent();
	simon->reset();
	simon->doUntouchable();
	game->setCameraPosition(initCam.x, initCam.y);
}

void Stage::loadContent()
{
	loadObjectFromFiles();
}

void Stage::loadObjectFromFiles()
{
	fstream fs;
	const wstring objectsPath = STAGE_PREFIX_PATH + mapName + STAGE_OBJECTS_PATH;
	fs.open(objectsPath.c_str(), ios::in);
	if (fs.fail())
	{
		DebugOut(L"[ERROR] Scene %d load data failed: file path = %s\n", mapId, objectsPath.c_str());
		fs.close();
		return;
	}

	int id;
	float x, y;

	// read all file
	while (!fs.eof())
	{
		// read line
		fs >> id >> x >> y;
		switch (id)
		{
		case OBSimon:
		{
			float max, min, camX, camY;
			fs >> min >> max >> camX >> camY;
			simon->setPosition(x, y);
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
			const auto candle = CandleFactory::Get()->getCandle(type, itemContainType, itemNx, { x, y },
				getGrid());
			auto unit = new Unit(getGrid(), candle, x, y);
			DebugOut(L"\n load candle");
			break;
		}

		case OBChangeStage:
		{
			float width, height;
			int nextStage;
			fs >> width >> height >> nextStage;
			auto obj = new ObjectChangeStage();
			obj->setWidthHeight(width, height);
			obj->setPosition(x, y);
			obj->setNextStage(nextStage);
			auto unit = new Unit(getGrid(), obj, x, y);
			DebugOut(L"\n load obChangeStage");
			break;
		}

		case OBEnemy:
		{
			int type, faceSide, respawnTime;
			float min, max;
			fs >> faceSide >> type >> min >> max >> respawnTime;
			auto obj = EnemyFactory::getInstance()->getEnemy(type);
			obj->setRespawnTime(respawnTime);
			obj->setRespawnArea({ min, max });
			obj->setInitPos({ x, y });
			obj->setPosition(x, y);
			obj->setInitFaceSide(faceSide);
			obj->setFaceSide(faceSide);
			auto unit = new Unit(getGrid(), obj, x, y);
			DebugOut(L"\n load enemy");
			break;
		}
		case OBDoor:
		{
			int nx;
			float min, max, moveCam;
			fs >> nx >> min >> max >> moveCam;
			auto obj = new Door();
			obj->setInitPos({ x, y });
			obj->setPosition(x, y);
			obj->setFaceSide(nx);
			obj->setNextCameraLimit({ min, max });
			obj->setMoveCamDistance(moveCam);
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
			ob->setPosition(x, y);
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
			ob->setPosition(x, y);
			DebugOut(L"load castle\n");
			auto unit = new Unit(getGrid(), ob, x, y);
		}
		default:
			break;
		}
	}

	fs.close();
}

void Stage::loadBoundaryCase(fstream& fs, float x, float y)
{
	float width, height;
	int type;
	fs >> width >> height >> type;
	auto boundary = BoundaryFactory::getInstance()->getBoundary(type);
	boundary->setWidhtHeight(width, height);
	boundary->setPosition(x, y);
	switch (type)
	{
	case BoundaryStair:
	{
		float stairType, faceSide, nextX, nextY;
		fs >> stairType >> faceSide >> nextX >> nextY;
		auto stair = dynamic_cast<Stair*>(boundary);
		if (stair)
		{
			stair->setFaceSide(faceSide);
			stair->setStairType(stairType);
			stair->setNextPos({ nextX, nextY });
			listStairs.push_back(stair);
		}
		break;
	}
	case BoundaryNormal:
	case BoundaryGround:
		listBoundary.push_back(boundary);
		break;
	default: auto unit = new Unit(grid, boundary, x, y);
	}
}

void Stage::render()
{
	TileMapManager::getInstance()->get(mapId)->draw();

	for (auto object : listRenderObj)
	{
		if (renderBoundingBox) object->renderBoundingBox();
		object->render();
	}
}

void Stage::update(DWORD dt)
{
	loadListObjFromGrid();

	const auto map = getMapSimonCanCollisionObjects();
	respawnEnemies();

	getSimon()->update(dt, map);
	for (auto obj : listRenderObj)
	{
		if (obj->getType() == OBSimon) continue;
		auto subWeapon = dynamic_cast<SubWeapon*>(obj);
		if (subWeapon)
		{
			updateSubWeapon(subWeapon, dt);
			continue;
		}

		auto enem = dynamic_cast<Enemy*>(obj);
		if (enem)
			enem->setIsStopAllAction(stopEnemyAction);

		obj->update(dt, &listBoundary);
	}
	updateCamera(dt);
	updateGrid();
	updateInActiveUnit();
}

void Stage::updateSubWeapon(SubWeapon* subWeapon, DWORD dt)
{
	auto simonPos = getSimon()->getPosition();
	auto holyWater = dynamic_cast<SubWeaponHolyWater*>(subWeapon);
	vector<GameObject*> temp = listCanHitObjects;
	temp.push_back(simon);
	temp.insert(temp.end(), listEnemy.begin(), listEnemy.end());
	if (holyWater)
	{
		temp.insert(temp.end(), listCanHitObjects.begin(), listCanHitObjects.end());
		vector<MapGameObjects> maps;
		maps.push_back({ OBEnemy, &temp});
		maps.push_back({ OBBoundary, &listBoundary});
		
		holyWater->update(dt, simonPos, simon->getState(), maps);
	}
	else
	{
		subWeapon->update(dt, simonPos, simon->getState(), &temp);
	}
}

void Stage::respawnEnemies()
{
	for (auto obj : listEnemy)
	{
		float playerX, playerY;
		simon->getPosition(playerX, playerY);
		auto enemy = dynamic_cast<Enemy*>(obj);
		if (enemy) enemy->respawn(playerX, playerY);
	}
}

vector<MapGameObjects> Stage::getMapSimonCanCollisionObjects()
{
	vector<MapGameObjects> map;
	map.push_back({ OBForceIdleSim, &listForceIdleSim });
	map.push_back({ OBStair, &listStairs });
	map.push_back({ OBItem, &listItems });
	map.push_back({ OBCanHitObjs, &listCanHitObjects });
	map.push_back({ OBChangeStage, &listObjectChangeStage });
	map.push_back({ OBEnemy, &listEnemy });
	map.push_back({ OBBoundary, &listStopSimObjs });
	return map;
}

void Stage::updateInActiveUnit()
{
	for (auto ob : listRenderObj)
	{
		auto wepon = dynamic_cast<SubWeapon*>(ob);
		if (!isInViewport(ob))
		{
			auto type = ob->getType();
			switch (type)
			{
			case OBSubWeapon:
				ob->setActive(false);
				ob->setEnable(false);
				break;
			case OBEnemy:
			{
				auto enemy = dynamic_cast<Enemy*>(ob);
				if (enemy->IsEnable())
				{
					enemy->setEnable(false);
					enemy->reset();
				}
				break;
			}
			default:;
			}
		}
	}
}

bool Stage::isInViewport(GameObject* object)
{
	const auto camPosition = Game::getInstance()->getCameraPosition();

	auto isInView = isColliding(object->getBoundingBox(),
		{
			camPosition.x, camPosition.y, camPosition.x + SCREEN_WIDTH,
			camPosition.y + SCREEN_HEIGHT
		});


	return isInView;
}

void Stage::updateGrid()
{
	for (auto unit : listUnit)
	{
		auto obj = unit->get();
		if (!obj->IsEnable() || !obj->IsActive()) continue;

		const auto pos = obj->getPosition();
		unit->move(pos.x, pos.y);
	}
}

bool sortByType(GameObject* a, GameObject* b)
{
	return a->getType() < b->getType();
}

void Stage::loadListObjFromGrid()
{
	resetAllList();
	listRenderObj = listBoundary;
	listStopSimObjs = listBoundary;
	listRenderObj.push_back(simon);
	listRenderObj.insert(listRenderObj.begin(), listStairs.begin(), listStairs.end());
	getGrid()->get(Game::getInstance()->getCameraPosition(), listUnit);

	for (auto unit : listUnit)
	{
		auto obj = unit->get();
		listRenderObj.push_back(obj);

		const auto type = obj->getType();
		switch (type)
		{
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
		default:;
		}
	}
	sort(listRenderObj.begin(), listRenderObj.end(), sortByType);
}

void Stage::resetAllList()
{
	listUnit.clear();
	listRenderObj.clear();
	listItems.clear();
	listCanHitObjects.clear();
	listEnemy.clear();
	listDoor.clear();
	listStopSimObjs.clear();
	listForceIdleSim.clear();
}

void Stage::updateCamera(const DWORD dt) const
{
	auto res = game->getCamera()->update(dt);
	if (res) return;
	float simonX, simonY, simonVx, simonVy;
	getSimon()->getPosition(simonX, simonY);
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

void Stage::onKeyDown(const int keyCode)
{
	getSimon()->handleOnKeyDown(keyCode);
	switch (keyCode)
	{
	case DIK_B: renderBoundingBox = !renderBoundingBox;
		break;
	case DIK_U: simon->powerUpWhip();
		break;
	case DIK_D: simon->powerUpWhip(false);
		break;
	case DIK_S: stopEnemyAction = !stopEnemyAction;
		break;
	case DIK_X: simon->getHurt(1, 1, 1);
		break;
	case DIK_C: simon->getHurt(1, 1, 666);
		break;
	case DIK_1: simon->setSubWeapon(itemDagger);
		break;
	case DIK_2: simon->setSubWeapon(itemAxe);
		break;
	case DIK_3: simon->setSubWeapon(itemBoomerang);
		break;
	case DIK_4: simon->setSubWeapon(itemHolyWater);
		break;
	case DIK_F: simon->updateEnergy(100);
		break;
	default:;
	}
}

void Stage::onKeyUp(const int keyCode) const
{
	getSimon()->handleOnKeyRelease(keyCode);
}

void Stage::keyState(BYTE* states) const
{
	getSimon()->handleOnKeyPress(states);
}

Grid* Stage::getGrid() const
{
	return grid;
}

Simon* Stage::getSimon() const
{
	return simon;
}

int Stage::getId()
{
	return mapId;
}
