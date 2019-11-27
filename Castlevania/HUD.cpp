#include "HUD.h"
HUD* HUD::instance = nullptr;


void HUD::add(string id, string sprite, unordered_map<string, Sprite*>& sprites) {
	auto spriteManager = SpriteManager::getInstance();
	sprites[id] = spriteManager->get(sprite);
}

void HUD::add(int id, const char* str, unordered_map<int, Sprite*>& pairs) {
	pairs[id] = SpriteManager::getInstance()->get(str);
}

void HUD::init() {
	auto game = Game::getInstance();
	font = game->getFont();


	stage = StageManager::getInstance()->getCurrentStage();
	add("blackboard", "blackboard_default_0", sprites);
	add("pause", "pause_0", sprites);
	add("lostHP", "HP_lose", sprites);
	add("playerHP", "HP_player", sprites);
	add("enemyHP", "HP_enemy", sprites);
	add("yesNo", "YesNo_default_0", sprites);
	add("chose", "item_largeheart", sprites);
	add(itemDagger, "item_dagger", subWeapons);
	add(itemAxe, "item_axe", subWeapons);
	add(itemBoomerang, "item_boomerang", subWeapons);
	add(itemHolyWater, "item_holywater", subWeapons);
	add(itemStopWatch, "item_stopwatch", subWeapons);
	add(1, "empty_ani_0", shotsType);
	add(2, "item_doubleshot", shotsType);
}

void HUD::update(DWORD dt) {
	int remainTime = StageManager::getInstance()->getRemainTime();

	auto score = StageManager::getInstance()->getScore();
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
	auto stageManager = StageManager::getInstance();
	if (stageManager->getIsGameOver()) showInfo();
	else showHud();

	auto offset = 50;
	auto pauseX = SCREEN_WIDTH - 34 - offset;
	auto pauseY = SCREEN_HEIGHT - 52 - offset;
	if (isGamePause) sprites["pause"]->draw(pauseX, pauseY);
}

void HUD::showInfo() {
	auto chose = StageManager::getInstance()->getPlayerChoseWhenOver();
	sprites["yesNo"]->draw(150, 150);
	sprites["chose"]->draw(160, 244 + 40 * chose);
}

void HUD::showHud() {
	if (font) font->DrawTextA(nullptr, info.c_str(), -1, &inforRect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
	sprites["blackboard"]->draw(0, 0);

	/*----------------- draw hp -----------------*/
	auto simonHP = 15;
	if (simon) simonHP = simon->getHp();
	auto boss = StageManager::getInstance()->getCurrentStage()->getBoss();
	auto bossHp = 15;
	if (boss) bossHp = boss->getHp();

	for (int i = 0; i < simonHP; i++)
		sprites["playerHP"]->draw(105 + i * 9, 31);

	for (int i = simonHP; i < SIM_MAX_HP; i++)
		sprites["lostHP"]->draw(105 + i * 9, 31);

	// enemy HP
	for (int i = 0; i < bossHp; i++)
		sprites["enemyHP"]->draw(105 + i * 9, 47);

	for (int i = bossHp; i < SIM_MAX_HP; i++)
		sprites["lostHP"]->draw(105 + i * 9, 47);

	auto subtype = -1;
	if (simon) {
		subtype = simon->getSubWeaponType();
	}
	if (subtype != -1) {
		subWeapons[subtype]->draw(1, 320, 38, 255, 255, 255, 255, 0);
	}

	auto shotType = simon->getCanShotTimes();
	shotsType[shotType]->draw(1, 450, 38);
}

