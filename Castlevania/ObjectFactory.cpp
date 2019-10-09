#include "ObjectFactory.h"
#include "Door.h"
#include "Boundary.h"


GameObject* ObjectFactory::getObj(int type)
{
	GameObject* ob;
	switch (type)
	{
		case OBDoor: ob = new Door();
			DebugOut(L"\n load door");
			break;
		default: ob= new Boundary();
	}

	return ob;
}

ObjectFactory* ObjectFactory::instance = nullptr;
