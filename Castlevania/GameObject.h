#pragma once
#include <Windows.h>
#include "AnimationManager.h"
#include "Game.h"
#include "SweptAABB.h"
#include "TextureManager.h"
#include "Timer.h"
using namespace std;

struct CheckPoint {
	int mapId;
	wstring mapName;
	float x;
	float y;
};

struct CollisionResult {
	bool x;
	bool y;
};

struct CollisionEvent;
typedef CollisionEvent* LPCollisionEvent;
class GameObject;
typedef GameObject* LPGAMEOBJECT;

class GameObject {
protected:
	int id;
	int type;

	float x;
	float y;
	float dx; // dx = vx*dt
	int alpha;
	int r;
	int g;
	int b;
	D3DXVECTOR2 initPos;
	float gravity;
	float dy; // dy = vy*dt
	float vx;
	float vy;
	D3DXVECTOR2 initVelocity;
	int hp;
	float boundingGameX; // gioi han man hinh game x
	float boundingGameY; // gioi han man hinh game y
	DWORD dt;
	int animId;
	int currentFrame;

	int faceSide;

	int state;
	int previousState;
	int initState;
	unordered_map<int, Animation*> animations;
	int preAnimId;
	bool previousAnimIsOneTimeAnim;
	bool isEnable;
	bool isActive;

	LPDIRECT3DTEXTURE9 texture;
	Animation* burnEffect;
	Timer* timerDeath = new Timer();
	Timer* timerDeflect = new Timer();
	Timer* timerUntouchable = new Timer();
	Timer* timerBurnEffect = new Timer();
	int untouchableDuration;
	float vxDeflect;
	float vyDeflect;
	int nxDeflect;
	int deflectTimeDuration;
	bool startDying;
	int deathTimeDuration;
	bool canDeflect;
	bool isInGround;
	float initGravity;
	string burnAnimId;
	bool isStopAllAction;
	int burningDuration;

public:
	GameObject();

	virtual ~GameObject();

	/*----------------- general  -----------------*/
	virtual void initAnim() = 0;
	virtual void render();
	virtual void updateAnimId();
	virtual void renderBoundingBox();
	void addAnimation(int id, string animTexId);

	void setDisable() {
		isActive = false;
		isEnable = false;
	}

	void removeAllVelocity() {
		gravity = 0;
		vx = 0;
		vy = 0;
	}

	/*----------------- hurt and death -----------------*/
	bool getHurt(int nx = -1, int hpLose = 1);
	virtual bool getHurt(int nx, int ny, int hpLose);

	virtual void doDeathAnim();

	void loseHp(int hpLose = 1);
	void doDeflect(int nx);
	void setStatusWhenStillHaveEnoughHP(int nx, int hpLose);
	void processUntouchableEffect();
	virtual void processWhenBurnedEffectDone();
	void doUntouchable();
	void doUntouchable(DWORD time);

	/*----------------- collide  -----------------*/

	LPCollisionEvent sweptAABBEx(LPGAMEOBJECT coO);
	void calcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects,
	                             vector<LPCollisionEvent>& coEvents);
	void calcPotentialCollisionsAABB(vector<LPGAMEOBJECT>* coObjects,
	                                 vector<LPCollisionEvent>& coEvents);
	void filterCollision(vector<LPCollisionEvent>& coEvents,
	                     vector<LPCollisionEvent>& coEventsResult, float& minTx,
	                     float& minTy, float& nx, float& ny);

	void checkCollisionWithWater(vector<LPGAMEOBJECT>* coObjects);
	/*----------------- update  -----------------*/

	virtual void update(DWORD dt, vector<GameObject*>* coObjects = nullptr);
	virtual void processDeflectEffect();

	void doActionAfterDeflect() {
		vx = 0;
		setState(idle);
		timerDeflect->stop();
		doUntouchable();
	}

	virtual void processDeathEffect();
	void updateGravity(DWORD dt, float gravity);
	void updateGravity(DWORD dt) { updateGravity(dt, this->gravity); };
	void updatePosWhenNotCollide();
	void updatePosInTheMomentCollideAndRemoveVelocity(float minTx, float minTy,
	                                                  float nx, float ny);
	void updatePosInTheMomentCollide(float minTx, float minTy, float nx,
	                                 float ny);

	void blockX(float minTx, float nx, float distance = 0.1) { x += minTx * dx + nx * distance; }
	void blockY(float minTy, float ny) { y += minTy * dy + ny * 0.1f; }

	void checkCollisionAndStopMovement(DWORD dt,
	                                   vector<GameObject*>* coObjects);

	/*----------------- bounding box  -----------------*/
	Box getBoundingBoxBaseOnFile();
	Box getBoundingBoxBaseOnFileAndPassWidth(float width);
	virtual Box getBoundingBox(float width, float height);

	virtual Box getBoundingBox() {
		if (!isEnable) return {0, 0, 0, 0};
		return getBoundingBox(-1, -1);
	};
	D3DXVECTOR2 getOffsetFromBoundingBox();

	/*----------------- get set   -----------------*/
	float getWidth() {
		const auto box = getBoundingBoxBaseOnFile();
		return box.r - box.l;
	}
	float getHeight() {
		const auto box = getBoundingBoxBaseOnFile();
		return box.b - box.t;
	}


	void setGravity(float val) { gravity = val; }
	void setType(int type) { this->type = type; }
	void setId(int id) { this->id = id; }

	void setPos(float x, float y) {
		this->x = x;
		this->y = y;
	}

	virtual void setState(int state) {
		this->previousState = this->state;

		this->state = state;
	}

	void setSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }

	void setBoundingGame(float x, float y) {
		this->boundingGameX = x;
		this->boundingGameY = y;
	}

	virtual D3DXVECTOR2 getCenter();

	int getId() const { return id; }
	int getType() const { return type; }
	int getPreviousState() const { return previousState; }
	int getState() const { return state; }
	void getSpeed(float& vx, float& vy) const;
	void getPos(float& x, float& y) const;
	D3DXVECTOR2 getPos() { return {x, y}; }
	void setPos(D3DXVECTOR2 pos) { x = pos.x; y = pos.y; }

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
	bool isDeflecting() { return isTimerRunning(timerDeflect) || state == deflect; }
	bool isUntouching() { return isTimerRunning(timerUntouchable); }
	bool isDying() { return isTimerRunning(timerDeath); };
	D3DXVECTOR2 getInitSpeed() const { return initVelocity; }
	void setInitSpeed(D3DXVECTOR2 val) { initVelocity = val; }
	int getFaceSide() const { return faceSide; }
	void setAnimId(int val) { animId = val; }
	int getInitState() const { return initState; }
	void setInitState(int val) { initState = val; }
	void doBurnedEffect();
	virtual bool processCollisionWithGround(float minTy, float ny);
	virtual bool processCollisionWithBoundaryByX(float minTx, float nx,
	                                             GameObject* boundary);
	float getInitGravity() const { return initGravity; }
	void setInitGravity(float val) { initGravity = val; }
	int getHp() const { return hp; }
	void setHp(int val) { hp = val; }
	bool getIsStopAllAction() const { return isStopAllAction; }
	void setIsStopAllAction(bool val) { isStopAllAction = val; }
	virtual void setDeathByWater();
	Timer* getTimerUntouchable() const { return timerUntouchable; }
};

inline Box GameObject::getBoundingBox(float width, float height) {
	// return width height default by file;
	if (width == -1 && height == -1) return getBoundingBoxBaseOnFile();
	// return if set width height get bounding box
	if (width != -1 && height == -1)
		return getBoundingBoxBaseOnFileAndPassWidth(width);

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

struct CollisionEvent {
	GameObject* obj;
	float t, nx, ny;

	CollisionEvent(const float t, const float nx, const float ny,
	               const LPGAMEOBJECT obj = nullptr) {
		this->t = t;
		this->nx = nx;
		this->ny = ny;
		this->obj = obj;
	}

	static bool compare(const LPCollisionEvent& a, LPCollisionEvent& b) {
		return a->t < b->t;
	}
};

struct MapGameObjects {
	int id;
	vector<GameObject*>* objs;
};
