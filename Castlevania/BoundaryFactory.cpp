#include "BoundaryFactory.h"
#include "Stair.h"

BoundaryFactory* BoundaryFactory::instance = nullptr;

Boundary* BoundaryFactory::getBoundary(int type)
{
	Boundary* boundary;
	switch (type)
	{
	case boundaryNormal:
		boundary = new Boundary();
		break;
	case boundaryStair:
		boundary = new Stair();
		break;
	default:
		boundary = new Boundary();
	}
	boundary->setBoundaryType(type);
	return boundary;
}