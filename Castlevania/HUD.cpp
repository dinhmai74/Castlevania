#include "HUD.h"
HUD* HUD::instance = nullptr;

void HUD::init() {
	auto spriteManager = SpriteManager::getInstance();
	auto game = Game::getInstance();
	defaultTime = DEFAULT_TIME_PLAY;
	time = 0;

	stage = StageManager::getInstance()->getCurrentStage();
	score = StageManager::getInstance()->getScore();
	blackboard = spriteManager->get("blackboard_default_0");
	pauseTexture = spriteManager->get("pause_0");
	font = game->getFont();
	subWeapons.push_back(spriteManager->get("item_dagger"));
	subWeapons.push_back(spriteManager->get("item_axe"));
	subWeapons.push_back(spriteManager->get("item_boomerang"));
	subWeapons.push_back(spriteManager->get("item_holywater"));
	subWeapons.push_back(spriteManager->get("item_stopwatch"));

	for (int i = 0; i < 16; i++) {
		playerHP.push_back(spriteManager->get("HP_player"));
		loseHP.push_back(spriteManager->get("HP_lose"));
		enemyHP.push_back(spriteManager->get("HP_enemy"));
	}
}

void HUD::update(DWORD dt, bool stopwatch) {
	if (!stopwatch) {
		time += dt;
	}
	int remainTime = defaultTime - time / CLOCKS_PER_SEC;
	if (remainTime <= 0) {
		remainTime = 0;
	}

	score = StageManager::getInstance()->getScore();
	simon = stage->getSimon();

	string scoreStr = to_string(score);
	while (scoreStr.length() < 6) scoreStr = "0" + scoreStr;

	string timeStr = to_string(remainTime);
	while (timeStr.length() < 4) timeStr = "0" + timeStr;

	auto stageStr = StageManager::getInstance()->getCurrentMapDisplayName();
	while (stageStr.length() < 2) stageStr = "0" + stageStr;

	string energyStr = to_string(simon->getEnergy());
	while (energyStr.length() < 2) energyStr = "0" + energyStr;

	string lifeStr = to_string(simon->Life());
	while (lifeStr.length() < 2) lifeStr = "0" + lifeStr;

	info = "SCORE-" + scoreStr + " TIME " + timeStr + " STAGE " + stageStr + "\n";
	info += "PLAYER                    " + energyStr + "\n";
	info += "ENEMY                     " + lifeStr + "\n";
	SetRect(&inforRect, 0, 15, SCREEN_WIDTH, 80);
}

void HUD::render() {
	showHud();

	auto offset = 50;
	auto pauseX = SCREEN_WIDTH - 34 -offset;
	auto pauseY = SCREEN_HEIGHT - 52 - offset;
	if (isGamePause) pauseTexture->draw(pauseX,pauseY);
}

void HUD::showInfo() {
}

void HUD::showHud() {
	if (font) font->DrawTextA(nullptr, info.c_str(), -1, &inforRect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
	blackboard->draw(0, 0);

	/*----------------- draw hp -----------------*/
	auto simonHP = simon->getHp();
	auto boss = StageManager::getInstance()->getCurrentStage()->getBoss();
	auto bossHp = 15;
	if (boss) bossHp = boss->getHp();

	for (int i = 0; i < simonHP; i++)
		playerHP[i]->draw(105 + i * 9, 31);

	for (int i = simonHP; i < SIM_MAX_HP; i++)
		loseHP[i]->draw(105 + i * 9, 31);

	// enemy HP
	for (int i = 0; i < bossHp; i++)
		enemyHP[i]->draw(105 + i * 9, 47);

	for (int i = bossHp; i < SIM_MAX_HP; i++)
		loseHP[i]->draw(105 + i * 9, 47);

	auto subtype = simon->getSubWeaponType();
	if (subtype != -1) {
		subWeapons[subtype]->draw(1, 320, 38, 255, 255, 255, 255, 0);
	}

}