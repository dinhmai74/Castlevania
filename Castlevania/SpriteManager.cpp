#include "SpriteManager.h"

SpriteManager * SpriteManager::instance = NULL;

void SpriteManager::add(const string id, float left, float top, float right, float bottom, float boxleft, float boxtop,
	float boxright, float boxbottom, LPDIRECT3DTEXTURE9 texture)
{
	auto sprite = new Sprite(id, left, top, right, bottom, boxleft, boxtop, boxright, boxbottom, texture);
	sprites[id] = sprite;
}
