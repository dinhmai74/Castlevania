#include "IntroScene.h"

IntroScene* IntroScene::instance = nullptr;

void IntroScene::update(DWORD dt)
{
	if(!getIsReady()) {
		if (animations["tile"]->isDone()) tileAnim = "tile_done";
	}
}

void IntroScene::renderWalkingSimonScene() {
	sprites["intro"]->draw(0, 0);
}
void IntroScene::renderMainMenuScene() {
	sprites["mainMenu"]->draw(0, 0);
	animations[tileAnim]->render(-1, 365, 213);
}

void IntroScene::render()
{
	if(getIsReady()) {
		renderWalkingSimonScene();
	}else {
		renderMainMenuScene();
	}
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
	this->simon->setPos(450, 335);
	this->simon->setFaceSide(SideLeft);
	this->simon->setState(walking);
	this->simon->doAutoWalkWithDistance(-230, SIM_AUTO_WALK_DISTANCE_VX, idleBack, -1);


	for (int i = 0; i < 3; i++) {
		Bat* bat = new Bat();
		bats.push_back(bat);
	}

	addSprite("mainMenu", "mainmenu_default_0");
	addSprite("intro", "intro_bg_0");
	addAnimation("tile", "mainmenu_tile_default_ani");
	addAnimation("tile_done", "mainmenu_tile_idle_ani");
	tileAnim = "tile";
}

