#include "GameObject.h"

class Boundary final : public GameObject
{
public:
	Boundary(float width, float height)
		: width(width),
		height(height)
	{
	}

	void initAnim() override;
	void render() override;
	virtual void getBoundingBox(float& left, float& top, float& right, float& bottom) override;
	virtual Box getBoundingBox() override;

private:
	float width;
	float height;
};
