#include "BoundaryFactory.h"

BoundaryFactory* BoundaryFactory::instance = nullptr;

Boundary* BoundaryFactory::getBoundary(int type)
{
	Boundary* boundary;
	switch (type)
	{
	case boundaryNormal:
		boundary = new Boundary();
		break;
	default:
		boundary = new Boundary();
	}
	boundary->setBoundaryType(type);
	return boundary;
}