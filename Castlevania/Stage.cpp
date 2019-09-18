#include "Stage.h"
#include "ItemFactory.h"

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
	this->grid = new Grid(map->getMapWidth(), map->getMapHeight(), 256, map->getMapHeight() / 2);
	loadContent();
}

void Stage::loadContent()
{
	// simon is special one load in game;
	simon = new Simon();
	simon->setState(State::idle);
	initSimonPos();

	loadObjectFromFiles();
}

void Stage::initSimonPos()
{
	auto game = Game::getInstance();
	switch (mapId)
	{
	case ID_SCENE_1:
		game->setCameraPosition(0, 0);
		simon->setPosition(10, 280);
		break;
	}
}

void Stage::loadObjectFromFiles()
{
	fstream fs;
	const wstring objectsPath = SCENE_PREFIX_PATH + mapName + SCENE_OBJECTS_PATH;
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
			// boundary
		case boundary:
			{
				float width, height;
				fs >> width >> height;
				auto boundary = new Boundary(width, height);
				boundary->setPosition(x, y);
				boundary->setEnable();
				listBoundary.push_back(boundary);
				break;
			}
		case item:
			{
				int type;
				fs >> type;
				auto item = ItemFactory::Get()->getItem(type, {x, y});
				auto unit = new Unit(grid, item, x, y);
				break;
			}
		case candle:
			{
				int type, itemContainType ,itemNx;
				fs >> type >> itemContainType>> itemNx;
				const auto candle = CandleFactory::Get()->getCandle(type, itemContainType,itemNx, {x, y}, grid);
				auto unit = new Unit(grid, candle, x, y);
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
		if (object->IsEnable())object->render();
	}

	if (renderBoundingBox) simon->renderBoundingBox();
	simon->render();
}

void Stage::update(DWORD dt)
{
	loadListObjFromGrid();

	const auto map = getMapSimonCanCollisionObjects();
	simon->update(dt, map);
	for (auto staticObj : listRenderObj)
	{
		staticObj->update(dt, &listBoundary);
	}
	updateCamera(dt);
	updateGrid();
}

vector<MapGameObjects> Stage::getMapSimonCanCollisionObjects()
{
	vector<MapGameObjects> map;
	map.push_back({boundary, &listBoundary});
	map.push_back({item, &listItems});
	map.push_back({canHitObjs, &listCanHitObjects});
	return map;
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
	grid->get(Game::getInstance()->getCameraPosition(), listUnit);

	for (auto unit : listUnit)
	{
		auto obj = unit->get();
		listRenderObj.push_back(obj);

		const auto item = dynamic_cast<Item*>(obj);
		if (item)
		{
			listItems.push_back(item);
			continue;
		}
		const auto candle = dynamic_cast<Candle*>(obj);

		const auto canHitObject = candle;
		if (canHitObject) listCanHitObjects.push_back(candle);
	}
}

void Stage::updateCamera(const DWORD dt) const
{
	auto game = Game::getInstance();
	const auto map = TileMapManager::getInstance()->get(mapId);
	const auto mapWidth = map->getMapWidth();
	float simonX, simonY, simonVx, simonVy;
	simon->getPosition(simonX, simonY);
	simon->getSpeed(simonVx, simonVy);

	const int offset = simonVx * dt;

	float posX, posY;
	game->getCameraPosition(posX, posY);

	const auto isCanMoveArea =
		simonX + offset > SCREEN_WIDTH / 2 &&
		simonX + offset + SCREEN_WIDTH / 2 < mapWidth;

	if (isCanMoveArea)
		posX = simonX + offset - SCREEN_WIDTH / 2;

	game->setCameraPosition(posX, posY);
}

void Stage::onKeyDown(const int keyCode)
{
	simon->handleOnKeyDown(keyCode);
	if (keyCode == DIK_B)
		this->renderBoundingBox = !this->renderBoundingBox;
	else if (keyCode == DIK_U) simon->upgradeWhipLv();
	else if (keyCode == DIK_D) simon->upgradeWhipLv(false);
	else if (keyCode == DIK_R) {
		init(mapId, mapName);
	}
}

void Stage::onKeyUp(const int keyCode) const
{
	simon->handleOnKeyRelease(keyCode);
}

void Stage::keyState(BYTE* states) const
{
	simon->handleOnKeyPress(states);
}
