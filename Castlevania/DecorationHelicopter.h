#pragma once
#include "GameObject.h"


class DecorationHelicopter final : public GameObject
{
public:
	DecorationHelicopter();
	~DecorationHelicopter();
	void initAnim() override;
	void update(DWORD dt, vector<GameObject*>* coObjects= nullptr) override;
protected:
	
private:
};
