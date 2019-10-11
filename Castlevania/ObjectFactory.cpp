#include "ObjectFactory.h"
#include "Door.h"
#include "Boundary.h"
#include "ForceIdleSim.h"
#include "TempRenderObj.h"


GameObject* ObjectFactory::getObj(int type)
{
	GameObject* ob;
	switch (type)
	{
	default: ob = new TempRenderObj();
	}

	return ob;
}

ObjectFactory* ObjectFactory::instance = nullptr;
