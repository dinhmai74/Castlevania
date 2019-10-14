#pragma once
#include "GameObject.h"

enum BoundaryType {
	BoundaryNormal,
	BoundaryGround,
	BoundaryStair,
	BoundaryCastle,
	BoundaryForceJump,
};

class Boundary : public GameObject
{
public:
	Boundary();
	virtual void init();
	void initAnim() override;
	void render() override;
	Box getBoundingBox() override;

	int getBoundaryType() const;
	void setBoundaryType(int val);
	void setWidhtHeight(float w, float h);
private:
	float width;
	float height;
	int boundaryType;
};
