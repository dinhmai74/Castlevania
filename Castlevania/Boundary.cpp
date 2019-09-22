#include "Boundary.h"


void Boundary::initAnim()
{
}

void Boundary::render()
{
}

void Boundary::getBoundingBox(float& left, float& top, float& right, float& bottom)
{
	auto box = getBoundingBox();
	left = box.l;
	right = box.r;
	top = box.t;
	bottom = box.b;
}

Box Boundary::getBoundingBox()
{
	return { x,y, x + width,y + height };
}
