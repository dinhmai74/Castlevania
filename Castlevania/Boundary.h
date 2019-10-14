#pragma once
#include "GameObject.h"

enum BoundaryType {
	BoundaryNormal,
	BoundaryGround,
	BoundaryStair,
	BoundaryCastle,
};

class Boundary : public GameObject
{
public:
	Boundary();
	void initAnim() override;
	void render() override;
	virtual Box getBoundingBox() override;

	int getBoundaryType() const;
	void setBoundaryType(int val);
	void setWidhtHeight(float w, float h);
private:
	float width;
	float height;
	int boundaryType;
	void init();
};
