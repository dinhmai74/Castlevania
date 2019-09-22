#include "Sprite.h"


Sprite::Sprite(std::string id, float left, float top, float right, float bottom, float boxleft, float boxtop, float boxright, float boxbottom,
	LPDIRECT3DTEXTURE9 texture)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = texture;
	this->bbox = { boxleft,boxtop,boxright,boxbottom };
}

void Sprite::draw(int nx, float x, float y, int alpha, int r, int g, int b,int accordingCam)
{
	if (!this || !texture) return;
	auto game = Game::getInstance();
	game->draw(accordingCam, nx, x, y, texture, getSpriteRect(), bbox, alpha, r, g, b);
}
