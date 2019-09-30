#pragma once
#include "Boundary.h"

class Stair: public Boundary
{
public:
	D3DXVECTOR2 getCanStandPos();
	Stair();
	~Stair();
protected:
	
private:
};
