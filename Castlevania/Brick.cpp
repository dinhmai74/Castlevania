#include "Brick.h"


Brick::Brick()
{
	type = GameObjectType::brick;
	initAnim();
}


Brick::~Brick()
{
}

void Brick::render()
{
	animations[ANIMATION_BRICK_IDLE]->render(1,x, y);
}


RECT Brick::getBoundingBox()
{
	return {static_cast<LONG>(x), static_cast<LONG>(y), static_cast<LONG>(x + BRICK_2_SIZE), static_cast<LONG>(y + BRICK_2_SIZE
		)
	};
}

void Brick::update(DWORD dt, vector<GameObject*>* coObjects)
{
}

void Brick::initAnim()
{
	addAnimation(ANIMATION_BRICK_IDLE);
}
