#include "IntroScene.h"
#include "StageManager.h"

IntroScene* IntroScene::instance = nullptr;

void IntroScene::update(DWORD dt) {
	if (!getIsReady()) {
		if (animations["tile"]->isDone()) tileAnim = "tile_done";
	}
	else {
		if (simon->getState() == idleBack) timerChangingState->startDeep();
		if (timerChangingState->isTimeUpAndRunAlr()) {
			StageManager::getInstance()->setPlaying(1);
			return;
		}
		if (helicopter) helicopter->update(dt);
		for (auto bat : bats) bat->update(dt);
		vector<MapGameObjects> temp;
		simon->update(dt, temp);
		simon->updatePosWhenNotCollide();

	}
}

void IntroScene::renderWalkingSimonScene() {
	sprites["intro"]->draw(0, 0);

	for (auto bat : bats) bat->render();
	helicopter->render();
	simon->render();
}

void IntroScene::renderMainMenuScene() {
	sprites["mainMenu"]->draw(0, 0);
	animations[tileAnim]->render(-1, 365, 213);
}

void IntroScene::render() {
	if (getIsReady()) renderWalkingSimonScene();
	else renderMainMenuScene();
}

void IntroScene::addSprite(string id, string spriteName) {
	auto spriteManager = SpriteManager::getInstance();
	sprites[id] = spriteManager->get(spriteName);
}

void IntroScene::addAnimation(string id, string name) {
	animations[id] = AnimationManager::getInstance()->get(name);
}

void IntroScene::init() {
	this->setIsReady(getIsReady());

	simon = new Simon();
	simon->setGravity(0);
	simon->setInitGravity(0);
	simon->setPos(450, 337);
	simon->setFaceSide(SideLeft);
	simon->setState(walking);
	simon->doAutoWalkWithDistance(-220, 0.04, idleBack, SideLeft);

	for (int i = 0; i < 3; i++) {
		Bat* bat = new Bat();
		bats.push_back(bat);
	}

	addSprite("mainMenu", "mainmenu_default_0");
	addSprite("intro", "intro_bg_0");
	addAnimation("tile", "mainmenu_tile_default_ani");
	addAnimation("tile_done", "mainmenu_tile_idle_ani");
	tileAnim = "tile";

	helicopter = new Helicopter();
	helicopter->setPos({ 450,100 });
}
