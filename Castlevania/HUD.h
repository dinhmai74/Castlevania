﻿#pragma once

#include <vector>
#include "StageManager.h"
#include <string>

using namespace std;

class HUD {
public:
	~HUD() { instance = nullptr; }

	static HUD* getInstance() {
		if (instance == nullptr)
			instance = new HUD;
		return instance;
	}

	void init();
	void update(DWORD dt);
	void render();

	// hiển thị thông tin khi ấn phím Enter
	void showInfo();
	// hiển thị thông tin máu, Exit ở góc trái màn hình
	void showHud();
	void setIsGamePause(bool val) { isGamePause = val; }
private:
	HUD() = default;
	static HUD* instance;
	Stage* stage;
	Simon* simon;
	unordered_map<int, Sprite*> subWeapons;
	unordered_map<string, Sprite*> sprites;
	unordered_map<int, Sprite*> shotsType;
	void add(string id, string sprite, unordered_map<string, Sprite*>& sprites);
	void add(int id, const char* str, unordered_map<int, Sprite*>& pairs);

	ID3DXFont* font;
	RECT inforRect;
	string info;
	bool isGamePause;
};
