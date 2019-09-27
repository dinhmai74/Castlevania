#pragma once
#include "SimonConstants.h"
#include "Weapon.h"

class Whip : public Weapon
{
	int lv;
	bool rendered;
	vector<bool> hitting;
public:
	Whip();
	~Whip();

	void checkEnemyCollisions(vector<LPGAMEOBJECT> coObject);
	// Inherited via GameObject
	void render() override;
	void setSide(int side);
	void refreshAnim();
	void refreshState() { setState(STATE_WHIP_DISAPPEAR); };
	void upgradeWhipLv(bool up = true);
	void initAnim() override;

	Box getBoundingBox() override;
	void updatePos(float simonX, float simonY, int simonState);
	void update(DWORD dt, float simonX, float simonY, vector<LPGAMEOBJECT>* coObject, int simonState);
};
