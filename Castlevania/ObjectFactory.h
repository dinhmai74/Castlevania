#pragma once
#include "GameObject.h"

class ObjectFactory
{
public:
	~ObjectFactory() { instance = nullptr; }

	static ObjectFactory* getInstance() 
	{
		if (instance == nullptr)
			instance = new ObjectFactory;
		return instance;
	}
	GameObject* getObj(int type);
private:
	ObjectFactory() = default;
	static ObjectFactory* instance;
};


