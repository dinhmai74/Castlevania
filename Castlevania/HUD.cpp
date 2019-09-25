#include "HUD.h"
HUD* HUD::instance = nullptr;

void HUD::Init()
{
	auto spriteManager = SpriteManager::getInstance();
	auto game = Game::getInstance();
	defaultTime = DEFAULT_TIME_PLAY;
	time = 0;
	score = 0;

	stage = StageManager::getInstance()->getCurrentStage();
	blackboard = spriteManager->get("blackboard_default_0");
	font = game->getFont();
	subWeapons.push_back(spriteManager->get("item_dagger"));
	subWeapons.push_back(spriteManager->get("item_axe"));

	for (int i = 0; i < 16; i++)
	{
		playerHP.push_back(spriteManager->get("HP_player"));
		loseHP.push_back(spriteManager->get("HP_lose"));
		enemyHP.push_back(spriteManager->get("HP_enemy"));
	}

}

void HUD::update(DWORD dt, bool stopwatch)
{

	if (!stopwatch) time += dt;
	int remainTime = defaultTime - time / CLOCKS_PER_SEC;
	if (remainTime <= 0)
	{
		remainTime = 0;
	}

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

void HUD::render()
{
	showHud();
}

void HUD::showInfo()
{
}

void HUD::showHud()
{
	if (font) font->DrawTextA(NULL, info.c_str(), -1, &inforRect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
	blackboard->draw(0, 0);

	/*----------------- draw hp -----------------*/
	auto simonHP = simon->getHp();
	for (int i = 0; i < simonHP; i++)
		playerHP[i]->draw(105 + i * 9, 31);

	for (int i = simonHP; i < SIM_MAX_HP; i++)
		loseHP[i]->draw(105 + i * 9, 31);

	// enemy HP
	for (int i = 0; i < 15; i++)
		enemyHP[i]->draw(105 + i * 9, 47);

	for (int i = 15; i < SIM_MAX_HP; i++)
		loseHP[i]->draw(105 + i * 9, 47);

	auto subtype = simon->getSubWeaponType();
	if (subtype!= -1)
	{
		subWeapons[subtype]->draw(1, 320, 35, 255, 255, 255, 255, 0);
	}
}
