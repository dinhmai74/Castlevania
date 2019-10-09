#pragma once
#include "GameObject.h"

class Door final :  public GameObject
{
public:
	Door();
	~Door();
	void initAnim() override;
protected:
	
private:
};
