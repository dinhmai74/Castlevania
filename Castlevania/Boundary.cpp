#include "Boundary.h"


void Boundary::initAnim()
{
}

void Boundary::render()
{
}

RECT Boundary::getBoundingBox()
{
	return {static_cast<LONG>(x), static_cast<LONG>(y), static_cast<LONG>(x + width), static_cast<LONG>(y + height)};
}
