#include "Sprite.h"


Sprite::Sprite(std::string id, int left, int top, int right, int bottom, int boxleft, int boxtop, int boxright, int boxbottom,
	LPDIRECT3DTEXTURE9 texture)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = texture;
	this->bbox = { left,top,right,bottom };
}

void Sprite::draw(int nx, float x, float y, int alpha)
{
	if (!this || !texture) return;
	auto game = Game::getInstance();
	game->draw(-nx, x, y, texture, getSpriteRect(), bbox, alpha);
}
