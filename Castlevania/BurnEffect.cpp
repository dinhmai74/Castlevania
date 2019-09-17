#include "BurnEffect.h"
#include "StageManager.h"


BurnEffect::BurnEffect()
{
	BurnEffect::initAnim();
}


BurnEffect::~BurnEffect()
= default;


void BurnEffect::render()
{
}


Box BurnEffect::getBoundingBox()
{
	return GameObject::getBoundingBox(10, 10);
}

void BurnEffect::update(DWORD dt, vector<GameObject*>* coObjects)
{
}

void BurnEffect::initAnim()
{
}
