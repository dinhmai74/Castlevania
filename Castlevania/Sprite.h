#pragma once
#include"Game.h"
#include "Constants.h"

class Sprite
{
private:
	std::string id;
	int left;
	int top;
	int right;
	int bottom;

	RECT bbox;

	LPDIRECT3DTEXTURE9 texture;
public:
	Sprite(std::string id, int left, int top, int right, int bottom, int boxleft, int boxtop,
	       int boxright, int boxbottom, LPDIRECT3DTEXTURE9 texture);
	RECT getSpriteRect() const { return {left,top,right,bottom}; }
	void draw(int nx,float x,float y,int alpha=255);
	RECT getBbox() const { return bbox; }
	void setBbox(RECT val) { bbox = val; }
};

