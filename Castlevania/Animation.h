#pragma once
#include "AnimationFrame.h"
#include "SpriteManager.h"
#include "Constants.h"
using namespace std;

enum AnimState {
	rendered = 1,
	notRenderedYet = -1
};

class Animation {
	DWORD lastFrameTime;
	int defaultTime;
	int currentFrame;
	bool isRendered;
	int state = -1;
	vector <AnimationFrame* > frames;
	DWORD animStartTime;
	bool forceDone;
public:
	Animation(int defaultTime = 100) {
		this->defaultTime = defaultTime;
		lastFrameTime = -1;
		currentFrame = -1;
		state = -1;
	}

	int getCurrentFrame();
	void add(string spriteId, DWORD time = 0);
	void render(int nx, float x, float y, int alpha = 255, int r = 255, int g = 255, int b = 255, bool isStop = false);
	void refresh();
	bool isDone();
	void setAniStartTime(DWORD t = GetTickCount()) { animStartTime = t; }
	void setRenderFrame(int frame) { currentFrame = frame; }
	bool isOver(DWORD dt) const { return GetTickCount() - animStartTime >= dt; }
	Box getFrameSprite();
	Box getFrameBoundingBox();
	void setForceDone(bool val = true) { forceDone = val; }
};
