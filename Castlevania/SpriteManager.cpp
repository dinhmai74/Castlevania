#include "SpriteManager.h"

SpriteManager * SpriteManager::instance = NULL;

void SpriteManager::add(std::string id, int left, int top, int right, int bottom, int boxleft, int boxtop, int boxright, int boxbottom, LPDIRECT3DTEXTURE9 texture)
{
	auto sprite = new Sprite(id, left, top, right, bottom, boxleft, boxtop, boxright, boxbottom, texture);
	sprites[id] = sprite;
}
