#pragma once
#include "GameObject.h"



class ObjectEndGame final : public GameObject
{
public:
	ObjectEndGame();
	~ObjectEndGame();
	void initAnim() override;
	void update(DWORD dt, vector<GameObject*>* coObjects) override;
protected:
	
private:
};
