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
	virtual Box getBoundingBox() override;

private:
	float width;
	float height;
};
