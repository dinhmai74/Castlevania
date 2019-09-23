#pragma once
#include "Game.h"
#include "AnimationManager.h"
#include "TextureManager.h"
#include "SweptAABB.h"
#include <Windows.h>
#include "Library/Inc/D3DX10math.h"
using namespace std;

constexpr auto FACE_TO_RIGHT = 1;
constexpr auto FACE_TO_LEFT = -1;

struct CollisionEvent;
typedef CollisionEvent* LPCollisionEvent;
class GameObject;
typedef GameObject* LPGAMEOBJECT;

class GameObject
{
protected:
	int id;
	int type;

	float x;
	float y;
	float dx;	// dx = vx*dt
	int alpha;
	int r;
	int g;
	int b;
	D3DXVECTOR2 initPos;
	float gravity;
	float dy;	// dy = vy*dt
	float vx;
	float vy;
	int hp;
	float boundingGameX; // gioi han man hinh game x
	float boundingGameY; // gioi han man hinh game y
	DWORD dt;
	int animId;
	int currentFrame;

	int faceSide;

	int currentState;
	int previousState;
	unordered_map<int, Animation*> animations;
	int preAnimId;
	bool previousAnimIsOneTimeAnim;
	bool isEnable;
	bool isActive;

	LPDIRECT3DTEXTURE9 texture;
	Animation* burnEffect;

	void createBlowUpEffectAndSetRespawnTimer();
	void processWhenBurnedEffectDone();
public:
	GameObject();

	virtual ~GameObject();

	virtual void initAnim() = 0;
	virtual void render();

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
	void getSpeed(float& vx, float& vy) const
	{
		vx = this->vx; vy = this->vy;
	}
	void getPosition(float& x, float& y) const
	{
		x = this->x;
		y = this->y;
	}

	D3DXVECTOR2 getPosition() { return{ x,y }; }

	virtual void renderBoundingBox();
	virtual void getHurt(int nx = 1, int hpLose = 1);
	virtual void getHurt(int nx ,int ny, int hpLose );
	void loseHp(int hpLose = 1);
	void setStatusWhenStillHaveEnoughHP(int hpLose);

	void addAnimation(int id, string animTexId);

	LPCollisionEvent sweptAABBEx(LPGAMEOBJECT coO);
	void calcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCollisionEvent>& coEvents);
	void calcPotentialCollisionsAABB(vector<LPGAMEOBJECT>* coObjects, vector<LPCollisionEvent>& coEvents);
	void filterCollision(
		vector<LPCollisionEvent>& coEvents,
		vector<LPCollisionEvent>& coEventsResult,
		float& minTx,
		float& minTy,
		float& nx,
		float& ny);

	virtual void update(DWORD dt, vector<GameObject*>* coObjects = nullptr);
	void updateGravity(float gravity);
	void updatePosWhenNotCollide();
	void updatePosInTheMomentCollide(float minTx, float minTy, float nx,
		float ny);
	void checkCollisionAndStopMovement(DWORD dt, vector<GameObject*>* coObjects);

	Box getBoundingBoxBaseOnFile();
	Box getBoundingBoxBaseOnFileAndPassWidth(float width);
	virtual Box getBoundingBox(float width, float height);
	virtual Box getBoundingBox() { return getBoundingBox(-1, -1); };
	D3DXVECTOR2 getOffsetFromBoundingBox();
	bool IsActive() const { return isActive; }
	void setActive(bool val = true) { isActive = val; }
	bool IsEnable() const { return isEnable; }
	void setEnable(bool val = true) { isEnable = val; }
	D3DXVECTOR2 getInitPos() const { return initPos; }
	void setInitPos(D3DXVECTOR2 val) { initPos = val; }
	void setFaceSide(int val) { faceSide = val; }
};

inline Box GameObject::getBoundingBox(float width, float height)
{	// return width height default by file;
	if (width == -1 && height == -1) return getBoundingBoxBaseOnFile();
	// return if set width height get bounding box
	if (width != -1 && height == -1) return getBoundingBoxBaseOnFileAndPassWidth(width);

	// lấy full box với width height tính từ trung tâm;
	const auto box = getBoundingBoxBaseOnFile();
	const auto boxWidth = box.r - box.l;
	const auto boxHeight = box.b - box.t;
	const float l = x + boxWidth / 2 - width / 2;
	const float r = l + width;
	const float t = y + boxHeight / 2 - height / 2;
	const float b = t + height;
	return Box(l, t, r, b);
}

struct CollisionEvent
{
	GameObject* obj;
	float t, nx, ny;
	CollisionEvent(const float t, const float nx, const float ny, const LPGAMEOBJECT obj = nullptr)
	{
		this->t = t; this->nx = nx; this->ny = ny; this->obj = obj;
	}

	static bool compare(const LPCollisionEvent& a, LPCollisionEvent& b)
	{
		return a->t < b->t;
	}
};

struct MapGameObjects
{
	int id;
	vector<GameObject*>* objs;
};
