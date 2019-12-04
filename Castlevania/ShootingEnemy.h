#pragma once
#pragma once
#include "Enemy.h"

class ShootingEnemy: public Enemy
{
protected:
	virtual void shoot();
	virtual bool canShoot();
	virtual void generateBullet();
	Timer* timerShooting = new Timer(1000);

};

