#pragma once
#include "Simon.h"
#include "Bat.h"
#include "Helicopter.h"

class IntroScene
{
public:
	~IntroScene() {
		instance = nullptr;	for (int i = 0; i < 3; i++)
			delete bats[i];

		bats.clear();

		delete helicopter;
	}

	static IntroScene* getInstance() 
	{
		if (instance == nullptr)
			instance = new IntroScene;
		return instance;
	}


	void init();
	void update(DWORD dt);
	void renderWalkingSimonScene();
	void renderMainMenuScene();
	void render();
	bool getIsReady() const { return isReady; }
	void setIsReady(bool val) { isReady = val; }
private:
	IntroScene() = default;
	static IntroScene* instance;
	vector<Bat*> bats;
	Simon* simon;
	Helicopter* helicopter;
	bool isReady;
	unordered_map<string, Sprite*> sprites;
	unordered_map<string, Animation*> animations;
	const char* tileAnim;
	void addSprite(string id, string spriteName);
	void addAnimation(string id, string name);
	Timer* timerChangingState= new Timer(2000);
};

