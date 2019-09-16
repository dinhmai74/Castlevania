#include "CollisionEffect.h"
#include "StageManager.h"


CollisionEffect::CollisionEffect()
{
	CollisionEffect::initAnim();
}


CollisionEffect::~CollisionEffect()
= default;

void CollisionEffect::render()
{
}

RECT CollisionEffect::getBoundingBox()
{
	return GameObject::getBoundingBox(30, 30);
}

void CollisionEffect::initAnim()
{
}
