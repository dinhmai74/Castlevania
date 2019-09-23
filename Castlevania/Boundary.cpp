#include "Boundary.h"


void Boundary::initAnim()
{
}

void Boundary::render()
{
}

Box Boundary::getBoundingBox()
{
	return { x,y, x + width,y + height };
}
