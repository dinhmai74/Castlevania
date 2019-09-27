#pragma once
#include "GameObject.h"
#include "Constants.h"

class Item :
	public GameObject
{
protected:
	int itemType;
public:
	void init();
	Item();
	~Item();

	void update(DWORD dt, vector<GameObject*>* boundary) override;
	void setItemType(const int type);
	int getItemType() const;
	Box getBoundingBox() override;
private:
	void initAnim() override;
	void checkCollision(DWORD dt, vector<GameObject*>* boundary);
	Timer* appearTimer;
	Timer* untouchableTimer;
	void checkDurationToDisapear();
};
