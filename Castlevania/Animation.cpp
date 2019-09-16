#include "Animation.h"

int Animation::getFrame()
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

void Animation::render(int nx,float x, float y,int alpha) {
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
				if (!isOneTimeAnim) currentFrame = 0;
				else
				{
					currentFrame = currentFrame - 1;
					state = AnimState::rendered;
				}
			}
		}
	}
	frames[currentFrame]->getSprite()->draw(nx,x, y,alpha);
}

bool Animation::isDone()
{
	return currentFrame == frames.size()-1;
}

void Animation::setIsOneTimeAnim(bool isOnetimeAnim)
{
	this->isOneTimeAnim = isOnetimeAnim;
}

RECT Animation::getFrameRect()
{

	if (currentFrame == -1)
		return frames[0]->getSprite()->getSpriteRect();

	return frames[currentFrame]->getSprite()->getSpriteRect();
}

RECT Animation::getFrameBBoxRect()
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
