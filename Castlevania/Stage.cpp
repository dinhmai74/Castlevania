#include "Stage.h"
#include "ItemFactory.h"
#include "BoundaryFactory.h"
#include "SubWeaponHolyWater.h"
#include "ObjectChangeStage.h"
#include "EnemyFactory.h"

Stage::Stage()
{
}

Stage::~Stage()
= default;

void Stage::init(int mapId, wstring mapName)
{
	this->renderBoundingBox = false;
	initMap(mapId, mapName);
	initSimon();
	loadContent();
}

void Stage::init(int mapId, wstring mapName, Simon * simon)
{
	this->renderBoundingBox = false;
	this->simon = simon;
	simon->reset();
	initMap(mapId, mapName);
	loadContent();
}

void Stage::initMap(int mapId, wstring mapName)
{
	this->mapId = mapId;
	this->mapName = std::move(mapName);
	const auto map = TileMapManager::getInstance()->get(mapId);
	this->grid = new Grid(map->getMapWidth(), 480, map->getTileWidth());
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
	loadContent();
	simon->reset();
	simon->doUntouchable();
	Game::getInstance()->setCameraPosition(initCam.x, initCam.y);
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
		case ObjectType::simon:
			float camX, camY;
			fs >> camX >> camY;
			simon->setPosition(x, y);
			simon->setInitPos({ x, y });
			Game::getInstance()->setCameraPosition(camX, camY);
			initCam = { camX, camY };
			break;
		case boundary:
		{
			loadBoundaryCase(fs, x, y);
			break;
		}
		case item:
		{
			int type;
			fs >> type;
			auto item = ItemFactory::Get()->getItem(type, { x, y });
			auto unit = new Unit(getGrid(), item, x, y);
			break;
		}
		case candle:
		{
			int type, itemContainType, itemNx;
			fs >> type >> itemContainType >> itemNx;
			const auto candle = CandleFactory::Get()->getCandle(type, itemContainType, itemNx, { x, y }, getGrid());
			auto unit = new Unit(getGrid(), candle, x, y);
			break;
		}

		case obChangeStage:
		{
			float width, height;
			int nextStage;
			fs >> width >> height >> nextStage;
			auto obj = new ObjectChangeStage();
			obj->setWidthHeight(width, height);
			obj->setPosition(x, y);
			obj->setNextStage(nextStage);
			auto unit = new Unit(getGrid(), obj, x, y);
			break;
		}

		case enemy:
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
			break;
		}
		default: break;
		}
	}

	fs.close();
}

void Stage::loadBoundaryCase(fstream & fs, float x, float y)
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

	if (renderBoundingBox)
	{
		getSimon()->renderBoundingBox();
		getSimon()->getWhip()->renderBoundingBox();
	}
	getSimon()->render();

	for (auto obj : listRenderOverrideSim)
	{
		if (renderBoundingBox) obj->renderBoundingBox();
		obj->render();
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
	updateInActiveUnit();
	updateGrid();
}

void Stage::updateSubWeapon(SubWeapon * subWeapon, DWORD dt)
{
	auto simonPos = getSimon()->getPosition();
	auto holyWater = dynamic_cast<SubWeaponHolyWater*>(subWeapon);
	vector<GameObject*> temp = listCanHitObjects;
	temp.push_back(simon);
	temp.insert(temp.end(), listEnemy.begin(), listEnemy.end());
	if (holyWater)
	{
		temp.insert(temp.end(), listBoundary.begin(), listBoundary.end());
		subWeapon->update(dt, simonPos, simon->getState(), &temp);
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
	map.push_back({ boundary, &listBoundary });
	map.push_back({ stair, &listStairs });
	map.push_back({ item, &listItems });
	map.push_back({ canHitObjs, &listCanHitObjects });
	map.push_back({ obChangeStage, &listObjectChangeStage });
	map.push_back({ enemy, &listEnemy });
	return map;
}

void Stage::updateInActiveUnit()
{
	for (auto ob : listRenderObj)
	{
		if (!isInViewport(ob))
		{
			auto type = ob->getType();
			switch (type)
			{
			case ObjectType::subWeapon:
				ob->setActive(false);
				ob->setEnable(false);
				break;
			case enemy:
			{
				auto enemy = dynamic_cast<Enemy*>(ob);
				if (enemy->IsEnable()) {
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

bool Stage::isInViewport(GameObject * object)
{
	const auto camPosition = Game::getInstance()->getCameraPosition();

	return isColliding(object->getBoundingBox(),
		{ camPosition.x, camPosition.y, camPosition.x + SCREEN_WIDTH, camPosition.y + SCREEN_HEIGHT });
}

void Stage::updateGrid()
{
	for (auto unit : listUnit)
	{
		auto obj = unit->get();
		if (!obj->IsEnable()) continue;

		const auto pos = obj->getPosition();
		unit->move(pos.x, pos.y);
	}
}

bool sortByType(Unit * a, Unit * b)
{
	return a->get()->getType() < b->get()->getType();
}

void Stage::loadListObjFromGrid()
{
	listUnit.clear();
	listRenderObj.clear();
	listItems.clear();
	listCanHitObjects.clear();
	listEnemy.clear();
	listRenderObj = listBoundary;
	listRenderObj.insert(listRenderObj.begin(), listStairs.begin(), listStairs.end());
	getGrid()->get(Game::getInstance()->getCameraPosition(), listUnit);
	sort(listUnit.begin(), listUnit.end(), sortByType);

	for (auto unit : listUnit)
	{
		auto obj = unit->get();
		listRenderObj.push_back(obj);

		const auto type = obj->getType();
		switch (type)
		{
		case boundary:
		{
			const auto bound = dynamic_cast<Boundary*>(obj);
			const auto boundType = bound->getBoundaryType();
			if (boundType == BoundaryCastle) listRenderOverrideSim.push_back(bound);
			break;
		}
		case item: listItems.push_back(obj);
			break;
		case obChangeStage: listObjectChangeStage.push_back(obj);
			break;
		case candle: listCanHitObjects.push_back(obj);
			break;
		case enemy: listEnemy.push_back(obj);
			break;
		default:;
		}
	}
}

void Stage::updateCamera(const DWORD dt) const
{
	auto game = Game::getInstance();
	const auto map = TileMapManager::getInstance()->get(mapId);
	const auto mapWidth = map->getMapWidth();
	const auto mapHeight = map->getMapHeight();
	float simonX, simonY, simonVx, simonVy;
	getSimon()->getPosition(simonX, simonY);
	getSimon()->getSpeed(simonVx, simonVy);

	const int offset = simonVx*dt+ 30;

	float posX, posY;
	game->getCameraPosition(posX, posY);

	const auto isCanMoveAreaX =
		simonX + offset > SCREEN_WIDTH / 2 &&
		simonX + offset-20 + SCREEN_WIDTH / 2 < mapWidth;

	if (isCanMoveAreaX)
		posX = simonX + offset - SCREEN_WIDTH / 2;

	/*if (simonY + offset > SCREEN_HEIGHT / 2 &&
		simonY + offset + SCREEN_HEIGHT / 2 < mapHeight){
		posY = simonY + offset - SCREEN_HEIGHT / 2 - HEADER_HEIGHT;
	}*/
	game->setCameraPosition(posX, posY);
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
	default:;
	}
}

void Stage::onKeyUp(const int keyCode) const
{
	getSimon()->handleOnKeyRelease(keyCode);
}

void Stage::keyState(BYTE * states) const
{
	getSimon()->handleOnKeyPress(states);
}
