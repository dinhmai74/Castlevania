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
	loadContent();
}

void Stage::render()
{
	TileMapManager::getInstance()->get(mapId)->draw();

	for (auto bound : listBoundary)
	{
		if (bound->IsEnable()) {
			bound->render();
			bound->renderBoundingBox();
		}
	}
}

void Stage::update(DWORD dt)
{
}

void Stage::loadContent()
{
	loadObjectFromFiles();
}

void Stage::loadObjectFromFiles()
{
	fstream fs;
	const wstring objectsPath = SCENE_PREFIX_PATH + mapName +SCENE_OBJECTS_PATH ;
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
		}
		default:;
		}
	}

	fs.close();
}

void Stage::onKeyDown(int keyCode)
{
}

void Stage::onKeyUp(int keyCode)
{
}

void Stage::keyState(BYTE* states)
{
}

void Stage::updateCamera(DWORD dt) const
{
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

