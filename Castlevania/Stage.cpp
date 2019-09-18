#include "Stage.h"
#include <utility>
#include "TileMapManager.h"
#include "Item.h"


Stage::Stage()
{
}

Stage::~Stage()
{
}

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

	loadObjectFromFiles();
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
	float posX, posY;

	// read all file
	while (!fs.eof())
	{
		// read line
		fs >> id >> posX >> posY;
		switch (id)
		{
			// boundary
		case BOUNDARY:
		{
			float width, height;
			fs >> width >> height;
			auto boundary = new Boundary(width, height);
			boundary->setPosition(posX, posY);
			boundary->setEnable();
			listBoundary.push_back(boundary);
			break;
		}
		case ITEM:
		{
			float type;
			fs >> type;
			auto item = new Item();
			item->setPosition(posX, posY);
			item->setInitPos({ posX, posY });
			item->setEnable();
			auto unit = new Unit(grid, item, posX, posY);
		}
		default: break;
		}
	}

	fs.close();
}

void Stage::render()
{
	TileMapManager::getInstance()->get(mapId)->draw();


	for (auto object : listStaticObj)
	{
		if (renderBoundingBox) object->renderBoundingBox();
		object->render();
	}

	if (renderBoundingBox)simon->renderBoundingBox();
	simon->render();
}

void Stage::update(DWORD dt)
{
	simon->update(dt, &listBoundary);
	for (auto staticObj : listStaticObj)
	{
		staticObj->update(dt, &listBoundary);
	}
	updateCamera(dt);
	updateGrid();
	loadListObjFromGrid();
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
	listStaticObj.clear();
	listStaticObj = listBoundary;
	grid->get(Game::getInstance()->getCameraPosition(), listUnit);

	for (auto unit : listUnit)
	{
		listStaticObj.push_back(unit->get());
	}
}

void Stage::updateCamera(DWORD dt)
{
	auto game = Game::getInstance();
	const auto map = TileMapManager::getInstance()->get(mapId);
	const auto mapWidth = map->getMapWidth();
	float simonX, simonY, simonVx, simonVy;
	simon->getPosition(simonX, simonY);
	simon->getSpeed(simonVx, simonVy);

	const int offset = dt * simonVx;

	float posX, posY;
	game->getCameraPosition(posX, posY);

	if (simonX + offset > SCREEN_WIDTH / 2 &&
		simonX + offset + SCREEN_WIDTH / 2 < mapWidth)
	{
		posX = simonX + offset - SCREEN_WIDTH / 2;
	}

	game->setCameraPosition(posX, posY);
}

void Stage::onKeyDown(int keyCode)
{
	simon->handleOnKeyDown(keyCode);
	if (keyCode == DIK_B)
		this->renderBoundingBox = !this->renderBoundingBox;
	else if (keyCode == DIK_U) simon->upgradeWhipLv();
	else if (keyCode == DIK_D) simon->upgradeWhipLv(false);
	else if (keyCode == DIK_R) init(mapId,mapName);
}

void Stage::onKeyUp(int keyCode)
{
	simon->handleOnKeyRelease(keyCode);
}

void Stage::keyState(BYTE* states)
{
	simon->handleOnKeyPress(states);
}
