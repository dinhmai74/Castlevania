#include "Whip.h"


Whip::Whip()
{
	lv = 1;
	animationId = -1;
	Whip::initAnim();
}


Whip::~Whip()
= default;

void Whip::checkEnemyCollisions(vector<LPGAMEOBJECT> coObjects)
{
	for (auto& coObject : coObjects)
	{
		if (isColliding(getBoundingBox(), coObject->getBoundingBox()))
		{
			coObject->setState(State::dead);
		}
	}
}

void Whip::render()
{
	animations[lv]->render(faceSide, x, y);

	const auto frame = animations[lv]->getFrame();

	if (frame == 0) setState(STATE_WHIP_READY_HIT);
	else if (frame == 1) setState(STATE_WHIP_START_HIT);
	else if (frame == 2) setState(STATE_WHIP_HITTING);
	else setState(STATE_WHIP_DISAPPEAR);
}


void Whip::update(DWORD dt, float simonX, float simonY, vector<LPGAMEOBJECT>* coObject, int simonState)
{
	GameObject::update(dt);
	setPosition(simonX-90, simonY);

	auto state = getState();
	if (state == STATE_WHIP_HITTING)
	{
		//checkEnemyCollisions(*coObject);
	}

}

void Whip::setSide(int side)
{
	faceSide = side;
}

void Whip::refreshAnim()
{
	if (animations[lv]) animations[lv]->refresh();
}

RECT Whip::getBoundingBox()
{
	return GameObject::getBoundingBox(-1, -1);
}


void Whip::upgradeWhipLv()
{
	if (lv < MAX_WHIP_LV) lv++;
}

void Whip::initAnim()
{
	addAnimation(1, "normalwhip_ani");
}
