#pragma once
#include "Game.h"
#include "AnimationManager.h"
#include "TextureManager.h"
#include "SweptAABB.h"
#include <Windows.h>
#include "Timer.h"
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
	D3DXVECTOR2 initSpeed;
	int hp;
	float boundingGameX; // gioi han man hinh game x
	float boundingGameY; // gioi han man hinh game y
	DWORD dt;
	int animId;
	int currentFrame;

	int faceSide;

	int state;
	int previousState;
	unordered_map<int, Animation*> animations;
	int preAnimId;
	bool previousAnimIsOneTimeAnim;
	bool isEnable;
	bool isActive;

	LPDIRECT3DTEXTURE9 texture;
	Animation* burnEffect;
	Timer* timerDeath;
	Timer* timerDeflect;
	Timer* timerUntouchable;
	Timer* timerBurnEffect;
	int untouchableDuration;
	float vxDeflect;
	float vyDeflect;
	int nxDeflect;
	int deflectTimeDuration;
	bool startDying;
	bool startUntouch;
	int deathTimeDuration;
public:
	GameObject();

	virtual ~GameObject();

	/*----------------- general  -----------------*/
	virtual void initAnim() = 0;
	virtual void render();
	virtual void updateAnimId();
	virtual void renderBoundingBox();
	void addAnimation(int id, string animTexId);
	void setDisable() { isActive = false; isEnable = false; }

	/*----------------- hurt and death -----------------*/
	virtual void getHurt(int nx = -1, int hpLose = 1);
	virtual void getHurt(int nx, int ny, int hpLose);

	void doDeathAnim();

	void loseHp(int hpLose = 1);
	void doDeflect(int nx);
	void setStatusWhenStillHaveEnoughHP(int nx, int hpLose);
	void processUntouchableEffect();
	void createBlowUpEffectAndSetRespawnTimer();
	virtual void processWhenBurnedEffectDone();
	void doUntouchable();

	/*----------------- collide  -----------------*/

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
	/*----------------- update  -----------------*/

	virtual void update(DWORD dt, vector<GameObject*>* coObjects = nullptr);
	virtual void processDeflectEffect();
	virtual void processDeathEffect();
	void updateGravity(float gravity);
	void updateGravity() { updateGravity(this->gravity); };
	void updatePosWhenNotCollide();
	void updatePosInTheMomentCollideAndRemoveVelocity(float minTx, float minTy, float nx,
		float ny);
	void updatePosInTheMomentCollide(float minTx, float minTy, float nx,
		float ny);

	void blockX(float minTx, float nx)
	{
		x += minTx * dx + nx * 0.2f;
	}

	void blockY(float minTy, float ny)
	{
		y += minTy * dy + ny * 0.2f;
	}

	void checkCollisionAndStopMovement(DWORD dt, vector<GameObject*>* coObjects);

	/*----------------- bounding box  -----------------*/
	Box getBoundingBoxBaseOnFile();
	Box getBoundingBoxBaseOnFileAndPassWidth(float width);
	virtual Box getBoundingBox(float width, float height);
	virtual Box getBoundingBox()
	{
		if (!isEnable) return { 0,0,0,0 };
		return getBoundingBox(-1, -1);
	};
	D3DXVECTOR2 getOffsetFromBoundingBox();

	/*----------------- get set   -----------------*/
	void setGravity(float val) { gravity = val; }
	void setType(int type) { this->type = type; }
	void setId(int id) { this->id = id; }
	void setPosition(float x, float y) { this->x = x; this->y = y; }
	void setState(int state)
	{
		this->previousState = this->state; this->state = state;
	}
	void setSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void setBoundingGame(float x, float y) { this->boundingGameX = x; this->boundingGameY = y; }
	int getId() const { return id; }
	int getType() const { return type; }
	int getPreviousState() const { return previousState; }
	int getState() const { return state; }
	void getSpeed(float& vx, float& vy) const;
	void getPosition(float& x, float& y) const;
	D3DXVECTOR2 getPosition() { return{ x,y }; }

	bool IsActive() const { return isActive; }
	virtual void setActive(bool val = true) { isActive = val; }
	bool IsEnable() const { return isEnable; }
	virtual void setEnable(bool val = true) { isEnable = val; }
	D3DXVECTOR2 getInitPos() const { return initPos; }
	void setInitPos(D3DXVECTOR2 val) { initPos = val; }
	void setFaceSide(int val) { faceSide = val; }
	int NxDeflect() const { return nxDeflect; }
	void setNxDeflect(int val) { nxDeflect = val; }
	float VyDeflect() const { return vyDeflect; }
	void setVyDeflect(float val) { vyDeflect = val; }
	int DeflectTimeDuration() const { return deflectTimeDuration; }
	void setDeflectTimeDuration(int val) { deflectTimeDuration = val; }
	bool isTimerRunning(Timer* timer) { return timer->isRunning(); }
	bool isDeflecting() { return isTimerRunning(timerDeflect); }
	bool isUntouching() { return isTimerRunning(timerUntouchable); }
	bool isDying() { return isTimerRunning(timerDeath); };
	D3DXVECTOR2 getInitSpeed() const { return initSpeed; }
	void setInitSpeed(D3DXVECTOR2 val) { initSpeed = val; }
	int getFaceSide() const { return faceSide; }
private:
	void doBurnedEffect();
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
