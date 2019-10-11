#pragma once
#include "GameObject.h"
#include "SimonConstants.h"

class TempRenderObj : public GameObject
{
public:
	TempRenderObj();
	~TempRenderObj();
	void initAnim() override;
protected:

private:
};

inline TempRenderObj::TempRenderObj()
{
	initAnim();
	setAnimId(ANIM_IDLE);
}

inline TempRenderObj::~TempRenderObj()
{
}

inline void TempRenderObj::initAnim()
{
	addAnimation(ANIM_IDLE, "temp_idle");
}

