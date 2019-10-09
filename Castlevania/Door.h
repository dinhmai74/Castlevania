#pragma once
#include "GameObject.h"

enum DoorState
{
	DoorCloseIdle,
	DoorOpenIdle,
	DoorOpening,
	DoorClosing
};

class Door final :  public GameObject
{
public:
	Door();
	~Door();
	void initAnim() override;

	void updateAnimId() override;

	void update(DWORD dt, vector<GameObject*>* coObjects = nullptr) override;

	void openDoor();
	void closeDoor();

	bool isOpenning() { return state == DoorOpening || state== DoorOpenIdle; }
	bool isOpened() { return state == DoorOpenIdle; }

	Region getNextCameraLimit() const;

	void setNextCameraLimit(Region val);
	float getMoveCamDistance();
	void setMoveCamDistance(float val) { moveCamDistance = val; }

private:
	Region nextCameraLimit;
	float moveCamDistance;
};
