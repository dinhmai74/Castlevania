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

	Sprite* get(std::string id) {
		return sprites[id];
	}

	void add(const string id, float left, float top, float right, float bottom, float boxleft, float boxtop,
	        float boxright, float boxbottom, LPDIRECT3DTEXTURE9 texture);

	static SpriteManager * getInstance() {
		if (instance == NULL) instance = new SpriteManager();
		return instance;
	}
};

