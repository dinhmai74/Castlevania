#include "Stage.h"
#include <utility>
#include "TileMapManager.h"


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
	loadContent();
}

void Stage::render()
{
	TileMapManager::getInstance()->get(mapId)->draw();

	if (renderBoundingBox)
		for (auto bound : listBoundary)
		{
			bound->renderBoundingBox();
		}

	simon->render();
	if (renderBoundingBox) simon->renderBoundingBox();
}

void Stage::update(DWORD dt)
{
	simon->update(dt, &listBoundary);

	updateCamera(dt);
}

void Stage::loadContent()
{
	// simon is special one load in game;
	simon = new Simon();
	simon->setState(SimonState::idle);

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
		case 0:
		{
			float width, height;
			fs >> width >> height;
			auto boundary = new Boundary(width, height);
			boundary->setPosition(posX, posY);
			boundary->SetEnable();
			listBoundary.push_back(boundary);
			break;
		}
		default:break;
		}
	}

	fs.close();
}

void Stage::onKeyDown(int keyCode)
{
	simon->handleOnKeyDown(keyCode);
	if (keyCode == DIK_B)
		this->renderBoundingBox = !this->renderBoundingBox;
	else if (keyCode == DIK_U) simon->upgradeWhipLv();
	else if (keyCode == DIK_D) simon->upgradeWhipLv(false);
}

void Stage::onKeyUp(int keyCode)
{
	simon->handleOnKeyRelease(keyCode);
}

void Stage::keyState(BYTE* states)
{
	simon->handleOnKeyPress(states);
}

void Stage::updateCamera(DWORD dt) const
{
	auto game = Game::getInstance();
	const auto map = TileMapManager::getInstance()->get(mapId);
	const auto mapWidth = map->getMapWidth();
	float simonX, simonY, simonVx, simonVy;
	simon->getPosition(simonX, simonY);
	simon->getSpeed(simonVx, simonVy);

	const int offset = dt*simonVx;

	float posX, posY;
	game->getCameraPosition(posX, posY);

	if (simonX + offset > SCREEN_WIDTH / 2 &&
		simonX + offset + SCREEN_WIDTH / 2 < mapWidth) {
		posX = simonX + offset - SCREEN_WIDTH / 2;
	}

	game->setCameraPosition(posX, posY);
}

void Stage::add(GameObject* gameObject)
{
}

void Stage::addObjectToList(GameObject* gameObject, vector<GameObject*>& container)
{
}

void Stage::remove(GameObject* gameObject)
{
}

void Stage::removeObjectFromList(GameObject* gameObject, vector<GameObject*>& container)
{
}

