#include "StageLv1.h"
#include "BigCandle.h"
#include "Brick.h"
#include "StageManager.h"
#include "StageLv2.h"


StageLv1::StageLv1()
{
	sceneWidth = SCENE1_WIDTH;
	sceneHeight = SCENE1_HEIGHT;
	StageLv1::loadContent();
}


StageLv1::~StageLv1()
{
}

void StageLv1::render()
{
	tileMap->draw();

	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->render();
	}

	for (auto i = 0; i < canHitObjects.size(); i++)
	{
		auto canHitOb = canHitObjects[i];
		canHitOb->render();
	}
	this->simon->render();

	for (auto i = 0; i < coObjects.size(); i++)
	{
		auto coOb = coObjects[i];
		coOb->render();
	}

}

void StageLv1::update(DWORD dt)
{
	simon->update(dt, &coObjects, &canHitObjects);

	for (auto& item : items)
	{
		item->update(dt, &bricks);
	}

	updateCamera(dt);

	float simX, simY;
	simon->getPosition(simX, simY);
	if (simX > 1500) {
		StageManager::getInstance()->setStage(new StageLv2());
	}
}

void StageLv1::loadContent()
{
	auto simon = new Simon();

	simon->setPosition(10.f, 100.f);
	simon->setState(SimonState::idle);
	simon->setBoundingGame(SCENE1_WIDTH, SCENE1_HEIGHT);

	this->simon = simon;

	for (auto i = 0; i < 100; i++)
	{
		auto brick = new Brick();
		brick->setPosition(0 + i * 16.0f, LV1_GROUND_Y);
		brick->renderBoundingBox();
		add(brick);
	}

}

void StageLv1::onKeyDown(int keyCode)
{
	simon->handleOnKeyDown(keyCode);
}

void StageLv1::onKeyUp(int keyCode)
{
	simon->handleOnKeyRelease(keyCode);
}

void StageLv1::keyState(BYTE* states)
{
	simon->handleOnKeyPress(states);
}

void StageLv1::add(GameObject* gameObject)
{
	const auto type = gameObject->getType();
	if (type == GameObjectType::coObject) {
		addObjectToList(gameObject, coObjects);
	}
	else if (type == GameObjectType::item)
	{
		addObjectToList(gameObject, coObjects);
		addObjectToList(gameObject, items);
	}
	else if (type == GameObjectType::brick)
	{
		addObjectToList(gameObject, coObjects);
		addObjectToList(gameObject, bricks);
	}
	else if (type == GameObjectType::canHitObject) addObjectToList(gameObject,canHitObjects);
	else gameObjects.push_back(gameObject);
}

void StageLv1::addSimon(Simon* simon)
{
	this->simon = simon;
}

void StageLv1::remove(GameObject* gameObject)
{
	const auto type = gameObject->getType();

	if (type == GameObjectType::coObject) {
		removeObjectFromList(gameObject,coObjects);
	}
	else if(type== GameObjectType::item)
	{
		removeObjectFromList(gameObject,coObjects);
		removeObjectFromList(gameObject,items);
	}
	else if (type == GameObjectType::brick)
	{
		removeObjectFromList(gameObject, bricks);
	}
	else if(type==GameObjectType::canHitObject)
	{
		removeObjectFromList(gameObject, canHitObjects);
	}
	else
	{
		removeObjectFromList(gameObject,gameObjects);
	}

		delete(gameObject);
}
