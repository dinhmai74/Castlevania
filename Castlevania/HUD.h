#pragma once

#include <vector>
#include "StageManager.h"
#include <string>

using namespace std;

class HUD
{
public:
	~HUD() { instance = nullptr; }

	static HUD* getInstance()
	{
		if (instance == nullptr)
			instance = new HUD;
		return instance;
	}
	void init();
	void update(DWORD dt, bool stopwatch = false);
	void render();

	// hiển thị thông tin khi ấn phím Enter
	void showInfo();
	// hiển thị thông tin máu, Exit ở góc trái màn hình
	void showHud();
	void setTime(int val) { time = val; }
private:
	HUD() = default;
	static HUD* instance;
	Stage* stage;
	Simon* simon;
	DWORD time;
	DWORD defaultTime;
	int score;
	vector<Sprite*> playerHP;
	vector<Sprite*> enemyHP;
	vector<Sprite*> loseHP;
	vector<Sprite*> subWeapons;

	Sprite* blackboard;

	ID3DXFont* font;
	RECT inforRect;
	string info;
};
