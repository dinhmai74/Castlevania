#pragma once
#include "Animation.h"
using namespace std;

class AnimationManager {
private:
	static AnimationManager* instance;
	unordered_map<string, Animation*>  animations;
public:
	void add(string id, Animation* animation) {
		animations[id] = animation;
	}
	Animation* get(string id) {
		return animations[id];
	}

	static AnimationManager* getInstance();
};
