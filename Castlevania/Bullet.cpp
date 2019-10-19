#include "Bullet.h"


Bullet::~Bullet() = default;

void Bullet::initAnim()
{
	addAnimation(idle, "bullet_fireball_ani");
}

void Bullet::updateSpeed()
{
	vx = initVelocity.x * faceSide;
}

Bullet::Bullet()
{
	initAnim();
	setAnimId(idle);
	initVelocity = { 0.15,0 };
}

void Bullet::update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	this->dt = dt;
	dx = vx * dt;
	dy = vy * dt;

	x += dx;
	y += dy;

	updateSpeed();
}



