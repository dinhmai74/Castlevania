#include "StageLv2.h"
#include "Brick.h"


StageLv2::StageLv2()
{
	sceneWidth = SCENE2_WIDTH;
	sceneHeight = SCENE2_HEIGHT;
	Game::getInstance()->setCameraPosition(0, 0);
	StageLv2::loadContent();
}


StageLv2::~StageLv2()
{
}

void StageLv2::render()
{
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

void StageLv2::update(DWORD dt)
{
	simon->update(dt, &coObjects, &canHitObjects);

	for (auto& item : items)
	{
		item->update(dt, &bricks);
	}

	updateCamera(dt);

	float simX, simY;
	simon->getPosition(simX, simY);
}

void StageLv2::loadContent()
{
	auto simon = new Simon();

	simon->setPosition(0, 100.f);
	simon->setState(SimonState::idle);
	simon->setBoundingGame(SCENE2_WIDTH, SCENE2_HEIGHT);

	this->simon = simon;

		auto brick = new Brick();
		brick->setPosition(0 +  16.0f, LV1_GROUND_Y);
		add(brick);


}

void StageLv2::onKeyDown(int keyCode)
{
	simon->handleOnKeyDown(keyCode);
}

void StageLv2::onKeyUp(int keyCode)
{
	simon->handleOnKeyRelease(keyCode);

}

void StageLv2::keyState(BYTE* states)
{
	simon->handleOnKeyPress(states);
}

void StageLv2::add(GameObject* gameObject)
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
	else if (type == GameObjectType::canHitObject) addObjectToList(gameObject, canHitObjects);
	else gameObjects.push_back(gameObject);
}

void StageLv2::remove(GameObject* gameObject)
{
	const auto type = gameObject->getType();

	if (type == GameObjectType::coObject) {
		removeObjectFromList(gameObject, coObjects);
	}
	else if (type == GameObjectType::item)
	{
		removeObjectFromList(gameObject, coObjects);
		removeObjectFromList(gameObject, items);
	}
	else if (type == GameObjectType::brick)
	{
		removeObjectFromList(gameObject, bricks);
	}
	else if (type == GameObjectType::canHitObject)
	{
		removeObjectFromList(gameObject, canHitObjects);
	}
	else
	{
		removeObjectFromList(gameObject, gameObjects);
	}

	delete(gameObject);
}
