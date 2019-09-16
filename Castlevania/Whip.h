#pragma once
#include "GameObject.h"
#include "SimonConstants.h"


class Whip : public GameObject
{
	int lv;
public:
	Whip();
	~Whip();

	void checkEnemyCollisions(vector<LPGAMEOBJECT> coObject);
	// Inherited via GameObject
	virtual void render() override;
	void setSide(int side);
	void refreshAnim();
	void upgradeWhipLv(bool up=true);
	void initAnim() override;

	virtual RECT getBoundingBox() override;
	void updatePos(float simonX, float simonY, int simonState);
	void update(DWORD dt, float simonX, float simonY, vector<LPGAMEOBJECT>* coObject, int simonState);
};

