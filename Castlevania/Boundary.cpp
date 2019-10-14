#include "Boundary.h"

Boundary::Boundary()
{
	Boundary::init();
}

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

int Boundary::getBoundaryType() const
{
	return boundaryType;
}

void Boundary::setBoundaryType(int val)
{
	boundaryType = val;
}

void Boundary::setWidhtHeight(float w, float h)
{
	width = w; height = h;
}

void Boundary::init()
{
	boundaryType = BoundaryNormal;
	type = OBBoundary;
}