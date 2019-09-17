#pragma once
#include "Box.h"

bool isColliding(float bl, float bt, float br, float bb, float sl, float st, float sr, float sb);
bool isColliding(Box box1, Box box2);

void sweptAABB(
	float ml,			// move left 
	float mt,			// move top
	float mr,			// move right 
	float mb,			// move bottom
	float sl,			// static left
	float st,
	float sr,
	float sb,
	float dx, float dy, // distance in delta time can move
	float &t, float &nx, float &ny); // time,nx,ny  collide
void sweptAABB(Box movingBox, Box staticBox, float dx, float dy, float& t, float& nx, float& ny);