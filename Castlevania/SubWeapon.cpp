#include "SubWeapon.h"
#include "Candle.h"

SubWeapon::SubWeapon()
{
	init();
}

void SubWeapon::init()
{
	initAnim();
}

void SubWeapon::initAnim()
{
	addAnimation(itemDagger, "subweapons_dagger_ani");
}
