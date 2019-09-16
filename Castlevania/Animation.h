#pragma once
#include "AnimationFrame.h"
#include "SpriteManager.h"
#include "Constants.h"
using namespace std;

enum AnimState
{
	rendered = 1,
	notRenderedYet = -1
};

class Animation
{
	DWORD lastFrameTime;
	int defaultTime;
	int currentFrame;
	bool isRendered;
	int state = -1;
	vector <AnimationFrame* > frames;
	bool isOneTimeAnim;
public:
	Animation(int defaultTime=100) {
		this->defaultTime = defaultTime;
		lastFrameTime = -1;
		currentFrame = -1;
		state = -1;
	}

	int getFrame();
	void add(string spriteId, DWORD time = 0);
	void render(int nx, float x, float y, int alpha=255);
	void refresh();
	bool isDone();
	void setIsOneTimeAnim(bool isOnetimeAnim=true);
	RECT getFrameRect();
	RECT getFrameBBoxRect();
};
