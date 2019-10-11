#include "Door.h"

Door::Door()
{
	setType(OBDoor);
	initAnim();
	setState(DoorCloseIdle);
	setAnimId(DoorCloseIdle);
}

void Door::initAnim()
{
	addAnimation(DoorCloseIdle, "door_close_idle_ani");
	addAnimation(DoorOpening, "door_open_ani");
	addAnimation(DoorClosing, "door_close_ani");
	addAnimation(DoorOpenIdle, "door_open_idle_ani");
}

void Door::updateAnimId()
{
	animId = state;
}

void Door::update(DWORD dt, vector<GameObject*>* coObjects /*= nullptr*/)
{
	updateAnimId();

	if (state == DoorOpening && animations[DoorOpening]->isDone()) setState(DoorOpenIdle);
	if (state == DoorClosing&& animations[DoorClosing]->isDone()) setState(DoorCloseIdle);
}

void Door::openDoor()
{
	if(state== DoorCloseIdle) setState(DoorOpening);
}

void Door::closeDoor()
{
	if (state == DoorOpenIdle) setState(DoorClosing);
}



void Door::setNextCameraLimit(Region val)
{
	nextCameraLimit = val;
}

Region Door::getNextCameraLimit() const
{
	return nextCameraLimit;
}

float Door::getMoveCamDistance()
{
	return moveCamDistance;
}

Door::~Door() = default;
