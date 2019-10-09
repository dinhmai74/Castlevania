#pragma once
#include "GameObject.h"

class ObjectChangeStage : public GameObject
{
public:
	ObjectChangeStage();
	~ObjectChangeStage();
	void initAnim() override;
	void render() override;
	Box getBoundingBox() override;
	float NextStage() const { return nextStage; }
	void setNextStage(float val) { nextStage = val; }
	float Height() const { return height; }
	void setWidthHeight(float w, float h) { height = h; width = w; }
	float Width() const { return width; }
private:
	float width, height;
	float nextStage;
};

inline ObjectChangeStage::ObjectChangeStage()
{
	type = OBChangeStage;
	setEnable();
}

inline ObjectChangeStage::~ObjectChangeStage()
{
}

inline void ObjectChangeStage::render()
{
}

inline Box ObjectChangeStage::getBoundingBox()
{
	return { x,y, x + width, y + height };
}

inline void ObjectChangeStage::initAnim()
{
}
