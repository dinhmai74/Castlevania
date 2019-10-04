#include "BoundaryFactory.h"
#include "Stair.h"

BoundaryFactory* BoundaryFactory::instance = nullptr;

Boundary* BoundaryFactory::getBoundary(int type)
{
	Boundary* boundary;
	switch (type)
	{
	case BoundaryNormal:
		boundary = new Boundary();
		break;
	case BoundaryStair:
		boundary = new Stair();
		break;
	default:
		boundary = new Boundary();
	}
	boundary->setBoundaryType(type);
	return boundary;
}