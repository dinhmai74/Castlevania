#pragma once
#include"Game.h"
#include "Constants.h"

class Sprite
{
private:
	std::string id;
	float left;
	float top;
	float right;
	float bottom;

	Box bbox;

	LPDIRECT3DTEXTURE9 texture;
public:
	Sprite(std::string id, float left, float top, float right, float bottom, float boxleft, float boxtop,
		float boxright, float boxbottom, LPDIRECT3DTEXTURE9 texture);
	Box getSpriteRect() const { return {left,top,right,bottom}; }
	void draw(int nx,float x,float y,int alpha=255);
	Box getBbox() const { return bbox; }
	void setBbox(Box val) { bbox = val; }
};

