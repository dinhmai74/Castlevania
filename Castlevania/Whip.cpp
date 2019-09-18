#pragma once
#include "Whip.h"
#include "Simon.h"


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
			coObject->setEnable(false);
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
	updatePos(simonX, simonY, simonState);

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
	if (animations[lv]) animations[lv]->refresh();
}

Box Whip::getBoundingBox()
{
	if (currentState != STATE_WHIP_HITTING) return { 0,0,0,0, };

	float left;
	const auto top = y + 15;
	const auto bottom = top + WHIP_BBOX_HEIGHT;
	const auto normalXOffset = 50;
	const auto longWhipXOffset = 20;
	if (faceSide == FaceSide::left)
	{
		left = lv == 3 ? x + longWhipXOffset : x + normalXOffset;
	}
	else {
		left = lv == 3 ? (240 - longWhipXOffset) - LONG_CHAIN_BBOX_WIDTH + x : (240 - normalXOffset) - WHIP_BBOX_WIDTH + x;
	}

	const auto right = lv == 3 ? left + LONG_CHAIN_BBOX_WIDTH : left + WHIP_BBOX_WIDTH;
	return { left,top,right,bottom };
}

void Whip::updatePos(float simonX, float simonY, int simonState)
{
	if (simonState == hittingWhenSitting)
		simonY += 15;
	setPosition(simonX - 90, simonY);
}


void Whip::upgradeWhipLv(bool up)
{
	if (lv < MAX_WHIP_LV && up) lv++;
	else if (lv > 0 && !up) lv--;
}

void Whip::initAnim()
{
	addAnimation(1, "normalwhip_ani");
	addAnimation(2, "shortchain_ani");
	addAnimation(3, "longchain_ani");
}
