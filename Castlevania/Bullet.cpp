#include "Bullet.h"


Bullet::~Bullet() = default;

void Bullet::initAnim()
{
	addAnimation(idle, "bullet_fireball_ani");
}

void Bullet::updateSpeed()
{
	vx = initVelocity.x * faceSide;
	vy = initVelocity.y;
}

D3DXVECTOR2 Bullet::getCenter()
{
	return { x,y - 20 };
}

void Bullet::doDeathAnim()
{
	setState(death);
	doBurnedEffect();
}

Bullet::Bullet()
{
	initAnim();
	setType(OBBullet);
	setAnimId(idle);
	initVelocity = { 0.15,0 };
}

void Bullet::update(DWORD dt, vector<LPGAMEOBJECT> * coObject)
{
	processWhenBurnedEffectDone();
	if (timerBurnEffect->isRunning()) return;
	this->dt = dt;
	dx = vx * dt;
	dy = vy * dt;

	x += dx;
	y += dy;

	updateSpeed();
}



