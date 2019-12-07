#include "ShootingEnemy.h"
#include "Bullet.h"
#include "StageManager.h"

void ShootingEnemy::shoot()
{

}

bool ShootingEnemy::canShoot()
{
	return timerShooting->isTimeUpAndRunAlr();
}

void ShootingEnemy::generateBullet()
{
	auto bullet = new Bullet();
	bullet->setFaceSide(faceSide);
	auto box = getBoundingBox();
	auto width = box.r - box.l;
	const auto xBullet = faceSide > 0 ? x + width + 5 : x;
	bullet->setPos(xBullet, y + 10);
	bullet->setInitPos({ xBullet, y + 10 });

	StageManager::getInstance()->add(bullet);
}
