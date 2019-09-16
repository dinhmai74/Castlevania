#include "AnimationManager.h"

AnimationManager* AnimationManager::instance = NULL;


AnimationManager * AnimationManager::getInstance()
{
	if (instance == nullptr) instance = new AnimationManager();
	return instance;
}