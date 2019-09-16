#pragma once
#include "Game.h"
#include "Constants.h"
#include <Windows.h>
#include "Sprite.h"


class SpriteManager
{
private:
	static SpriteManager * instance;

	std::unordered_map<string, Sprite* > sprites;

	LPDIRECT3DTEXTURE9 texture;
public:
	void add(std::string id, int left, int top, int right, int bottom, int boxleft, int boxtop, int boxright, int boxbottom,  LPDIRECT3DTEXTURE9 texture);

	Sprite* get(std::string id) {
		return sprites[id];
	}

	static SpriteManager * getInstance() {
		if (instance == NULL) instance = new SpriteManager();
		return instance;
	}
};

