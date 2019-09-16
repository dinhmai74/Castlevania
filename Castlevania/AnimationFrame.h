#pragma once
#include "Game.h"
#include "Sprite.h"
using namespace std;

class AnimationFrame
{
private:
	Sprite* sprite;
	DWORD time;
public:
	AnimationFrame(Sprite* sprite, int time) {
		this->sprite = sprite;
		this->time = time;
	}

	DWORD getTime() { return time; }
	Sprite* getSprite() { return sprite; }
};

