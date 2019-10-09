#pragma once
#include <d3d9.h>
#include <d3dx9.h>

struct Region
{
	float min;
	float max;
};

class Camera
{
public:
	~Camera() { instance = nullptr; }

	static Camera* getInstance()
	{
		if (instance == nullptr)
			instance = new Camera;
		return instance;
	}
	float getY() const { return y; }
	void setY(float val) { y = val; }
	float getX() const { return x; }
	void setX(float val) { x = val; }
	Region getLimitX() const { return limitX; }
	void setLimitX(Region val) { limitX = val; }
	void setLimitXMax(float val){ limitX.max = val; }
	void setPos(D3DXVECTOR2 pos);
	D3DXVECTOR2 getPos();
	bool update(DWORD dt);
	bool isMoving() { return movedDistance > 0; }
	void move(float distance, float vx= 0.15f);
	void reset();
private:
	Camera();
	static Camera* instance;
	float x, y;
	float movedDistance;
	float moveVx;
	Region limitX;
};

