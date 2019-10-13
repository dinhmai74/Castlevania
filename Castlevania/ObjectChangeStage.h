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
	float getNextStageId() const { return nextStageId; }
	void setNextStageId(float val) { nextStageId = val; }
	float Height() const { return height; }
	void setWidthHeight(float w, float h) { height = h; width = w; }
	float Width() const { return width; }
	D3DXVECTOR2 getChangeStateVelocity() const { return changeStateVelocity; }
	void setChangeStateVelocity(D3DXVECTOR2 val) { changeStateVelocity = val; }
	int getChangeStateAnimId() const { return changeStateAnimId; }
	void setChangeStateAnimId(int val) { changeStateAnimId = val; }
	D3DXVECTOR2 getChangeStateDestinationPoint() const { return changeStateDestinationPoint; }
	void setChangeStateDestinationPoint(D3DXVECTOR2 val) { changeStateDestinationPoint = val; }
	std::wstring getNextStageMapObjName() const { return nextStageMapObjName; }
	void setNextStageMapObjName(std::wstring val) { nextStageMapObjName = val; }
	VectorInt getSimonDirectCollide() const { return simonDirectCollide; }
	void setSimonDirectCollide(VectorInt val) { simonDirectCollide = val; }
private:
	float width, height;
	int nextStageId;
	wstring nextStageMapObjName;
	D3DXVECTOR2 changeStateDestinationPoint;
	D3DXVECTOR2 changeStateVelocity;
	int changeStateAnimId;
	VectorInt simonDirectCollide;
};

inline ObjectChangeStage::ObjectChangeStage()
{
	type = OBChangeStage;
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
