#include "Candle.h"



void Candle::initAnim() {
	addAnimation(candleBig, "bigcandle_ani");
	addAnimation(candleSmall, "smallcandle_ani");
}

void Candle::update(DWORD dt, vector<GameObject*>* coObjects) {
	GameObject::update(dt);
	GameObject::checkCollisionAndStopMovement(dt, coObjects);
	if (burnEffect && burnEffect->isOver(BURNED_DURATION - 100)) generateItem();
}

int Candle::CandleType() const {
	return candleType;
}

void Candle::setCandleType(int val) {
	candleType = val;
}

Box Candle::getBoundingBox() {
	if (!IsEnable()) return { 0,0,0,0 };
	return getBoundingBoxBaseOnFile();
}

void Candle::render() {
	GameObject::render();
}

Candle::Candle() {
	init();
}

Candle::~Candle() {}

void Candle::init() {
	initAnim();
	type = OBCandle;
	setAnimId(candleBig);
}
