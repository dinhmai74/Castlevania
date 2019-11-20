#pragma once
#include "Whip.h"
#include "Simon.h"
#include "Candle.h"

Whip::Whip() {
	Whip::initAnim();
	lv = 1;
	setAnimId(-1);
	type = OBwhip;
	setDmg(2);
}

Whip::~Whip()
= default;

void Whip::checkEnemyCollisions(vector<LPGAMEOBJECT> coObjects) {
	for (auto& coObject : coObjects) {
		if (isColliding(getBoundingBox(), coObject->getBoundingBox())) {
			coObject->getHurt(1, 1, getDmg());
		}
	}
}

void Whip::render() {
	animations[lv]->render(getFaceSide(), x, y);


}

void Whip::update(DWORD dt, float simonX, float simonY, vector<LPGAMEOBJECT> * coObject, int simonState, int simClimbDirect) {
	GameObject::update(dt);
	updatePos(simonX, simonY, simonState, simClimbDirect);
	const auto frame = animations[lv]->getCurrentFrame();

	auto hittingFrame = 2;

	if (lv >= MAX_WHIP_LV) {
		if (frame >= 9 && frame < 11) setState(STATE_WHIP_HITTING);
		else setState(STATE_WHIP_DISAPPEAR);
	}
	else {
		if (frame == hittingFrame) setState(STATE_WHIP_HITTING);
		else setState(STATE_WHIP_DISAPPEAR);
	}
	auto state = getState();

	DebugOut(L"state %d\n",state);

	if (state == STATE_WHIP_HITTING) {
		checkEnemyCollisions(*coObject);
	}
}

bool Whip::isMaxLv() {
	return lv == MAX_WHIP_LV;
}

void Whip::setSide(int side) {
	setFaceSide(side);
}

void Whip::refreshAnim() {
	if (animations[lv]) animations[lv]->refresh();
	rendered = false;
}

Box Whip::getBoundingBox() {
	if (state != STATE_WHIP_HITTING) return { 0,0,0,0, };

	float left;
	const auto top = y + 15;
	const auto bottom = top + WHIP_BBOX_HEIGHT;
	const auto normalXOffset = 50;
	const auto longWhipXOffset = 20;
	if (getFaceSide() == Side::SideLeft) {
		left = lv == MAX_WHIP_LV ? x + longWhipXOffset : x + normalXOffset;
	}
	else {
		left = lv == MAX_WHIP_LV ? (240 - longWhipXOffset) - LONG_CHAIN_BBOX_WIDTH + x : (240 - normalXOffset) - WHIP_BBOX_WIDTH + x;
	}

	const auto right = lv == MAX_WHIP_LV ? left + LONG_CHAIN_BBOX_WIDTH : left + WHIP_BBOX_WIDTH;
	return { left,top,right,bottom };
}

void Whip::updatePos(float simonX, float simonY, int simonState, int simonClimbDirect) {
	if (simonState == hittingWhenSitting)
		simonY += 15;
	setPos(simonX - 90, simonY);
}

void Whip::upgradeWhipLv(bool up) {
	if (lv < MAX_WHIP_LV && up) lv++;
	else if (lv > 1 && !up) lv--;

	if (lv <= 1) lv = 1;

}

void Whip::initAnim() {
	addAnimation(1, "normalwhip_ani");
	addAnimation(2, "shortchain_ani");
	addAnimation(3, "longchain_ani");
}