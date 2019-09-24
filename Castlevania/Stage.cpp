#include "Stage.h"
#include "ItemFactory.h"
#include "BoundaryFactory.h"
#include "ObjectChangeStage.h"

Stage::Stage()
{
}

Stage::~Stage()
= default;

void Stage::init(int mapId, wstring mapName)
{
	this->mapId = mapId;
	this->mapName = std::move(mapName);
	this->renderBoundingBox = false;
	const auto map = TileMapManager::getInstance()->get(mapId);
	this->grid = new Grid(map->getMapWidth(), map->getMapHeight());
	loadContent();
	simon->doAutoWalk();
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
	// simon is special one load in game;
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
			DebugOut(L"simon\n");
			fs >> camX >> camY;
			simon = new Simon();
			simon->setPosition(x, y);
			simon->setInitPos({ x, y });
			Game::getInstance()->setCameraPosition(camX, camY);
			initCam = { camX,camY };
			break;
		case boundary:
		{
			float width, height;
			int type;
			fs >> width >> height >> type;
			auto boundary = BoundaryFactory::getInstance()->getBoundary(type);
			boundary->setWidhtHeight(width, height);
			boundary->setPosition(x, y);
			boundary->setEnable();
			listBoundary.push_back(boundary);
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
			obj->setEnable();
			obj->setNextStage(nextStage);
			auto unit = new Unit(getGrid(), obj, x, y);
			break;
		}

		default: break;
		}
	}

	fs.close();
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
}

void Stage::update(DWORD dt)
{
	loadListObjFromGrid();

	const auto map = getMapSimonCanCollisionObjects();
	getSimon()->update(dt, map);
	for (auto obj : listRenderObj)
	{
		auto subWeapon = dynamic_cast<SubWeapon*>(obj);
		if (subWeapon)
		{
			auto simonPos = getSimon()->getPosition();
			subWeapon->update(dt, simonPos, getSimon()->getState(), &listCanHitObjects);
			continue;
		}

		obj->update(dt, &listBoundary);
	}
	updateCamera(dt);
	updateInActiveUnit();
	updateGrid();
}

vector<MapGameObjects> Stage::getMapSimonCanCollisionObjects()
{
	vector<MapGameObjects> map;
	map.push_back({ boundary, &listBoundary });
	map.push_back({ item, &listItems });
	map.push_back({ canHitObjs, &listCanHitObjects });
	map.push_back({ obChangeStage, &listObjectChangeStage });
	return map;
}

void Stage::updateInActiveUnit()
{
	for (auto object : listRenderObj)
	{
		if (!isInViewport(object))
		{
			auto subWeapon = dynamic_cast<SubWeapon*>(object);
			if (subWeapon)
			{
				subWeapon->setActive(false);
				subWeapon->setEnable(false);
			}
		}
	}
}

bool Stage::isInViewport(GameObject* object)
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

void Stage::loadListObjFromGrid()
{
	listUnit.clear();
	listRenderObj.clear();
	listItems.clear();
	listCanHitObjects.clear();
	listRenderObj = listBoundary;
	getGrid()->get(Game::getInstance()->getCameraPosition(), listUnit);

	for (auto unit : listUnit)
	{
		auto obj = unit->get();
		listRenderObj.push_back(obj);

		const auto type = obj->getType();
		switch (type)
		{
		case item: listItems.push_back(obj);
			break;
		case obChangeStage: listObjectChangeStage.push_back(obj);
			break;
		case candle: listCanHitObjects.push_back(obj);
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

	const int offset = 60;

	float posX, posY;
	game->getCameraPosition(posX, posY);

	const auto isCanMoveAreaX =
		simonX + offset > SCREEN_WIDTH / 2 &&
		simonX + offset + SCREEN_WIDTH / 2 < mapWidth;

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
	case DIK_X: simon->getHurt();
		break;
	case DIK_C: simon->getHurt(1, 1, 88);
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
