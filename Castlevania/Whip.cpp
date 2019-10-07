#pragma once
#include "Whip.h"
#include "Simon.h"
#include "Candle.h"

Whip::Whip()
{
	Whip::initAnim();
	lv = 1;
	setAnimId(-1);
	type = whip;
	setDmg(1);
}

Whip::~Whip()
= default;

void Whip::checkEnemyCollisions(vector<LPGAMEOBJECT> coObjects)
{
	for (auto& coObject : coObjects)
	{
		if (isColliding(getBoundingBox(), coObject->getBoundingBox()))
		{
			coObject->getHurt(1, 1, getDmg());
		}
	}
}

void Whip::render()
{
	animations[lv]->render(getFaceSide(), x, y);

	const auto frame = animations[lv]->getCurrentFrame();

	auto hittingFrame = 2;

	if (lv == 3) hittingFrame = 9;

	if (frame == hittingFrame) setState(STATE_WHIP_HITTING);
	else setState(STATE_WHIP_DISAPPEAR);
}

void Whip::update(DWORD dt, float simonX, float simonY, vector<LPGAMEOBJECT> * coObject, int simonState)
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
	setFaceSide(side);
}

void Whip::refreshAnim()
{
	if (animations[lv]) animations[lv]->refresh();
	rendered = false;
}

Box Whip::getBoundingBox()
{
	if (state != STATE_WHIP_HITTING) return { 0,0,0,0, };

	float left;
	const auto top = y + 15;
	const auto bottom = top + WHIP_BBOX_HEIGHT;
	const auto normalXOffset = 50;
	const auto longWhipXOffset = 20;
	if (getFaceSide() == Side::SideLeft)
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
	else if (lv > 1 && !up) lv--;

	setDmg(lv);
}

void Whip::initAnim()
{
	addAnimation(1, "normalwhip_ani");
	addAnimation(2, "shortchain_ani");
	addAnimation(3, "longchain_ani");
}