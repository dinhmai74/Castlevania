#include "BoundaryFactory.h"
#include "Stair.h"
#include "stage1_castle.h"
#include "ForceIdleSim.h"

BoundaryFactory* BoundaryFactory::instance = nullptr;

Boundary* BoundaryFactory::getBoundary(int type)
{
	Boundary* boundary;
	switch (type)
	{
	case BoundaryNormal:
		boundary = new Boundary();
			DebugOut(L"\n load oboundary ");
		break;
	case BoundaryStair:
		boundary = new Stair();
			DebugOut(L"\n load stair");
		break;
	default:
		boundary = new Boundary();
	}
	boundary->setBoundaryType(type);
	return boundary;
}