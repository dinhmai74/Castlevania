#include "GameObject.h"
#include "Candle.h"

class Weapon : public GameObject
{
	int dmg; // dmg that weapon can do;
public:
	void initAnim() override {};
	void setDmg(int val) { dmg = val; }
	int getDmg() const { return dmg; }
	void checkCollision(DWORD dt, vector<GameObject*>* vector);
	virtual void update(DWORD dt, D3DXVECTOR2 simonPos, int simonState, vector<GameObject*>* coObjects);
};

inline void Weapon::checkCollision(DWORD dt, vector<GameObject*>* coObjs)
{
	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;
	coEvents.clear();

	calcPotentialCollisions(coObjs, coEvents);

	// no collison
	// case that 
	if (coEvents.empty())
		updatePosWhenNotCollide();
	else
	{
		float minTx;
		float minTy;
		float nx = 0;
		float ny;
		filterCollision(coEvents, coEventsResult, minTx, minTy, nx, ny);

		updatePosInTheMomentCollide(minTx, minTy, nx, ny);

		for (auto& i : coEventsResult)
		{
			const auto object = (i->obj);
			auto candle = dynamic_cast<Candle*>(object);
			if (candle)candle->getHurt(nx, ny, getDmg());
		}

		setActive(false);
		setEnable(false);
	}

	for (auto& coEvent : coEvents) delete coEvent;
}

inline void Weapon::update(DWORD dt, D3DXVECTOR2 simonPos, int simonState, vector<GameObject*>* coObjects)
{
	GameObject::update(dt);
	checkCollision(dt, coObjects);
}
