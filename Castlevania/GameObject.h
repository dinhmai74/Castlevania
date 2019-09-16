#pragma once
#include "Game.h"
#include "AnimationManager.h"
#include "TextureManager.h"
#include "SweptAABB.h"
using namespace std;

constexpr auto FACE_TO_RIGHT = 1;
constexpr auto FACE_TO_LEFT = -1;

struct CollisionEvent;
typedef CollisionEvent* LPCollisionEvent;
class GameObject;
typedef GameObject* LPGAMEOBJECT;

enum GameObjectType {
	simon = 1000,
	brick = 1001,
	boundary = 0,
	coObject = 1002,
	canHitObject=1003, // just display and not collision with simon
	item = 1004
};

class GameObject
{
protected:
	int id;
	int type;

	float x;
	float y;
	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt
	float vx;
	float vy;
	float boundingGameX; // gioi han man hinh game x
	float boundingGameY; // gioi han man hinh game y
	DWORD dt;
	int animationId;

	int faceSide;

	int currentState;
	int previousState;
	unordered_map<int, Animation*> animations;
	int previousAmiId;
	bool previousAnimIsOneTimeAnim;
	bool isEnable;
	bool isInActive;

	LPDIRECT3DTEXTURE9 texture;

public:
	GameObject();

	virtual ~GameObject();

	virtual void initAnim()=0;
	virtual void render() = 0;

	void setType(int type) { this->type = type; }
	void setId(int id) { this->id = id; }
	void setPosition(float x, float y) { this->x = x; this->y = y; }
	void setState(int state)
	{
		this->previousState = this->currentState; this->currentState = state;
	}
	void setSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void setBoundingGame(float x, float y) { this->boundingGameX = x; this->boundingGameY = y; }

	int getId() const { return id; }
	int getType() const { return type; }
	int getPreviousState() const { return previousState; }
	int getState() const { return currentState; }
	void getSpeed(float &vx, float &vy) const
	{
		vx = this->vx; vy = this->vy;
	}
	void getPosition(float &x, float &y) const
	{
		x = this->x;
		y = this->y;
	}

	virtual void renderBoundingBox();

	void addAnimation(int id, string animTexId);

	LPCollisionEvent sweptAABBEx(LPGAMEOBJECT coO);
	void calcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCollisionEvent> & coEvents);
	void filterCollision(
		vector<LPCollisionEvent> &coEvents,
		vector<LPCollisionEvent> &coEventsResult,
		float &minTx,
		float &minTy,
		float &nx,
		float &ny);

	virtual void update(DWORD dt, vector<GameObject*> *coObjects = nullptr);

	RECT getBoundingBoxBaseOnFile();
	RECT getBoundingBoxBaseOnFileAndPassWidth(float width);
	virtual RECT getBoundingBox(float width, float height);
	virtual RECT getBoundingBox() = 0;
	bool IsInActive() const { return isInActive; }
	void setIsInActive(bool val) { isInActive = val; }
	bool IsEnable() const { return isEnable; }
	void SetEnable(bool val=true) { isEnable = val; }
};

inline RECT GameObject::getBoundingBox(float width, float height)
{	// return width height default by file;
	if (width == -1 && height == -1) return getBoundingBoxBaseOnFile();
	// return if set width height get bounding box
	if (width != -1 && height == -1) return getBoundingBoxBaseOnFileAndPassWidth(width);

	// lấy full box với width height tính từ trung tâm;
	const auto box = getBoundingBoxBaseOnFile();
	const auto boxWidth = box.right - box.left;
	const auto boxHeight = box.bottom - box.top;
	const auto l = x + boxWidth / 2 - width / 2;
	const auto r = l + width;
	const auto t = y + boxHeight / 2 - height / 2;
	const auto b = t + height;
	return {static_cast<LONG>(l), static_cast<LONG>(t), static_cast<LONG>(r), static_cast<LONG>(b)};
}

struct CollisionEvent
{
	GameObject* obj;
	float t, nx, ny;
	CollisionEvent(const float t, const float nx, const float ny, const LPGAMEOBJECT obj = nullptr)
	{
		this->t = t; this->nx = nx; this->ny = ny; this->obj = obj;
	}

	static bool compare(const LPCollisionEvent &a, LPCollisionEvent &b)
	{
		return a->t < b->t;
	}
};
