#include "BoundaryFactory.h"

BoundaryFactory* BoundaryFactory::instance = nullptr;

Boundary* BoundaryFactory::getBoundary(int type)
{
	switch (type)
	{
	case boundaryNormal:
		return new Boundary();
	default:
		return new Boundary();
	}
}