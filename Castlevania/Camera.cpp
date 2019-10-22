#include "Camera.h"

Camera* Camera::instance = nullptr;

D3DXVECTOR2 Camera::getPos()
{
	return { x,y };
}

bool Camera::update(DWORD dt)
{
	if(movedDistance>0)
	{
		auto dx = moveVx * dt;
		x += dx;
		movedDistance -= fabs(dx);
		return true;
	}

	return false;
}

void Camera::move(float distance, float vx/*= 0.15f*/)
{
	if (getIsLocked()) return;
	movedDistance = distance;
	moveVx = vx;
}

void Camera::reset()
{
	x = 0;
	y = 0;
	movedDistance = 0;
	isLocked = false;
}

void Camera::setPos(D3DXVECTOR2 pos)
{
	x = pos.x;
	y = pos.y;
}

Camera::Camera()
{
	x = y = 0;
	limitX = { 0,512 };
}
