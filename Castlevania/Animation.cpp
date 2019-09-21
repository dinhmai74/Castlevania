#include "Animation.h"

int Animation::getCurrentFrame()
{
	return currentFrame;
}

void Animation::add(string spriteId, DWORD time) {
	int t = time;
	if (time == 0) t = this->defaultTime;
	auto spriteManager = SpriteManager::getInstance();
	auto sprite = spriteManager->get(spriteId);
	auto frame = new AnimationFrame(sprite, t);
	frames.push_back(frame);
}

void Animation::render(int nx, float x, float y, int alpha, int r, int g, int b)
{
	const auto now = GetTickCount();
	if (currentFrame == -1) {
		currentFrame = 0;
		lastFrameTime = now;
	}
	else {
		const auto t = frames[currentFrame]->getTime();
		if (now - lastFrameTime > t) {
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) {
				currentFrame = 0;
			}
		}
	}
	frames[currentFrame]->getSprite()->draw(nx, x, y, alpha, r, b, g);

}

void Animation::render(int nx, float x, float y, int frame, int alpha, int r, int g, int b)
{
	frames[frame]->getSprite()->draw(nx, x, y, alpha, r, b, g);
}

bool Animation::isDone()
{
	return currentFrame == frames.size() - 1 || forceDone;
}


Box Animation::getFrameSprite()
{

	if (currentFrame == -1)
		return frames[0]->getSprite()->getSpriteRect();

	return frames[currentFrame]->getSprite()->getSpriteRect();
}

Box Animation::getFrameBoundingBox()
{
	if (currentFrame == -1)
		return frames[0]->getSprite()->getBbox();

	return frames[currentFrame]->getSprite()->getBbox();
}

void Animation::refresh()
{
	currentFrame = -1;
	state = AnimState::notRenderedYet;
}
