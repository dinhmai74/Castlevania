#include "Stair.h"



D3DXVECTOR2 Stair::getCanStandPos()
{
	const auto posX = x + 23;
	const auto posY = y - 18;

	return { posX,posY};
}

Stair::Stair()
{
	setBoundaryType(boundaryStair);
}

Stair::~Stair()
{

}
