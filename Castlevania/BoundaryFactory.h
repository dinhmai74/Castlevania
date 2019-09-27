#pragma once
#include "Boundary.h"

class BoundaryFactory
{
public:
	~BoundaryFactory() { instance = nullptr; }

	static BoundaryFactory* getInstance()
	{
		if (instance == nullptr)
			instance = new BoundaryFactory;
		return instance;
	}

	Boundary* getBoundary(int type);
private:
	BoundaryFactory() = default;
	static BoundaryFactory* instance;
};
