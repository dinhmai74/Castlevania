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
	return;
	if (lv == 3)
	{
		animationId = faceSide == FaceSide::left
			              ? ANIM_WHIP_LV3_L
			              : ANIM_WHIP_LV3_R;
	}
	else if (lv == 2)
	{
		animationId = faceSide == FaceSide::left
			              ? ANIM_WHIP_LV2_L
			              : ANIM_WHIP_LV2_R;
	}
	else
	{
		animationId = faceSide == FaceSide::left
			              ? ANIM_WHIP_LV1_L
			              : ANIM_WHIP_LV1_R;
	}

	animations[animationId]->render(1, x, y);

	const auto frame = animations[animationId]->getFrame();

	if (frame == 0) setState(STATE_WHIP_READY_HIT);
	else if (frame == 1) setState(STATE_WHIP_START_HIT);
	else if (frame == 2) setState(STATE_WHIP_HITTING);
	else setState(STATE_WHIP_DISAPPEAR);
}


void Whip::update(DWORD dt, float x, float y, vector<LPGAMEOBJECT>* coObject)
{
	return;
	GameObject::update(dt);
	setPosition(x, y);

	auto state = getState();
	if (state == STATE_WHIP_HITTING)
	{
		checkEnemyCollisions(*coObject);
	}
}

void Whip::setSide(int side)
{
	faceSide = side;
}

void Whip::refreshAnim()
{
	if (animations[animationId]) animations[animationId]->refresh();
}

RECT Whip::getBoundingBox()
{
	float left, top, right, bottom;
	if (currentState == STATE_WHIP_HITTING)
	{
		if (faceSide == FaceSide::left)
		{
			if (lv == 1) left = x - WHIP_LV1_HITTING_W;
			else if (lv == 2) left = x - WHIP_LV2_HITTING_W;
			else if (lv == 3) left = x - WHIP_LV3_HITTING_W;
			else left = 0;

			right = x + 5;
		}
		else
		{
			left = x + SIM_HIT_W - 5;
			if (lv == 1) right = x + SIM_HIT_W + WHIP_LV1_HITTING_W - 5;
			else if (lv == 2) right = x + SIM_HIT_W + WHIP_LV2_HITTING_W - 5;
			else if (lv == 3) right = x + SIM_HIT_W + WHIP_LV3_HITTING_W - 5;
			else right = 0;
		}

		top = y + 20;
		bottom = top + WHIP_LV1_HITTING_H;
	}
	else
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}
	return {static_cast<LONG>(left), static_cast<LONG>(top), static_cast<LONG>(right), static_cast<LONG>(bottom)};
}

void Whip::upgradeWhipLv()
{
	if (lv < MAX_WHIP_LV) lv++;
}

void Whip::initAnim()
{
}
