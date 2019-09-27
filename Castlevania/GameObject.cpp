#include "GameObject.h"
#include <algorithm>

void GameObject::processWhenBurnedEffectDone()
{
	if(timerBurnEffect->isTimeUpAndRunAlr())
	{
		burnEffect = nullptr;
		if (state == death) setActive(false);
		timerBurnEffect->stop();
	}
}

void GameObject::doUntouchable()
{
	timerUntouchable->start();
}

GameObject::GameObject()
{
	x = y = 0;
	initPos = { 0, 0 };
	vx = vy = 0;
	alpha = r = b = g = 255;
	setFaceSide(right); // right side
	preAnimId = -1;
	previousAnimIsOneTimeAnim = false;
	boundingGameX = 0;
	boundingGameY = 0;
	state = normal;
	isActive = true;
	gravity = 0;
	hp = 1;
	untouchableDuration = 2000;
	deflectTimeDuration = 400;
	deathTimeDuration = 1000;
	timerUntouchable = new Timer(untouchableDuration);
	timerDeflect = new Timer(deflectTimeDuration);
	timerDeath = new Timer(deathTimeDuration);
	timerBurnEffect = new Timer(BURNED_DURATION);
	vxDeflect = 0.15f;
	vyDeflect = 0.06f;
	nxDeflect = -1;
}

GameObject::~GameObject()
{
	if (texture != nullptr) texture->Release();
}

void GameObject::updateAnimId()
{
	switch (state)
	{
	case deflect:
		if (animations[ANIM_DEFLECT]) animId = ANIM_DEFLECT;
		break;
	case death:
		if (animations[ANIM_DEATH]) animId = ANIM_DEATH;
		break;
	default:;
	}
}

void GameObject::render()
{
	if (burnEffect)
	{
		const auto blowX = x;
		const auto blowY = y;
		burnEffect->render(1, blowX, blowY);
	}
	if (!IsEnable()) return;
	animations[animId]->render(faceSide, x, y, alpha);
}

void GameObject::getSpeed(float& vx, float& vy) const
{
	vx = this->vx;
	vy = this->vy;
}

void GameObject::getPosition(float& x, float& y) const
{
	x = this->x;
	y = this->y;
}

void GameObject::renderBoundingBox()
{
	const auto texture = TextureManager::getInstance()->get(ID_TEX_BBOX);
	auto game = Game::getInstance();
	float l, r, t, b;
	const auto rect = getBoundingBox();

	game->draw(faceSide, rect.l, rect.t, texture, rect, rect, 140);
}

void GameObject::addAnimation(int id, string animTexId)
{
	auto animation = AnimationManager::getInstance()->get(animTexId);
	animations[id] = animation;
}

void GameObject::getHurt(int nx, int hpLose)
{
	GameObject::getHurt(nx, 1, hpLose);
}

void GameObject::getHurt(int nx, int ny, int hpLose)
{
	if (isUntouching() || isDeflecting()) return;
	if (this->hp <= hpLose)
	{
		setState(death);
		this->hp = 0;
		timerDeath->start();
	}
	else setStatusWhenStillHaveEnoughHP(nx, hpLose);
}

void GameObject::loseHp(int hpLose)
{
	hp -= hpLose;
	if (hp <= 0)
		hp = 0;
}

void GameObject::setStatusWhenStillHaveEnoughHP(int nx, int hpLose)
{
	loseHp(hpLose);
	setState(deflect);
	if (nx == 0) nx = 1;
	setNxDeflect(nx);
	timerDeflect->start();
}

LPCollisionEvent GameObject::sweptAABBEx(LPGAMEOBJECT coO)
{
	float t, nx, ny;

	auto coBox = coO->getBoundingBox();

	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	coO->getSpeed(svx, svy);

	float sdx = svx * dt;
	float sdy = svy * dt;

	float dx = this->dx - sdx;
	float dy = this->dy - sdy;

	auto obBox = getBoundingBox();

	sweptAABB(
		obBox,
		coBox,
		dx, dy,
		t, nx, ny
	);

	auto* e = new CollisionEvent(t, nx, ny, coO);
	return e;
}

/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void GameObject::calcPotentialCollisions
(vector<LPGAMEOBJECT>* coObjects, vector<LPCollisionEvent>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		auto e = sweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CollisionEvent::compare);
}

void GameObject::calcPotentialCollisionsAABB(vector<LPGAMEOBJECT>* coObjects, vector<LPCollisionEvent>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		auto ob = coObjects->at(i);
		auto isCollided = isColliding(getBoundingBox(), ob->getBoundingBox());
		if (isCollided)
		{
			auto* e = new CollisionEvent(0, 1, 1, ob);
			coEvents.push_back(e);
		}
	}
}

void GameObject::filterCollision
(vector<LPCollisionEvent>& coEvents,
	vector<LPCollisionEvent>& coEventsResult,
	float& min_tx, float& min_ty, float& nx, float& ny)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		auto c = coEvents[i];

		if (c->t < min_tx && c->nx != 0)
		{
			min_tx = c->t;
			nx = c->nx;
			min_ix = i;
		}

		if (c->t < min_ty && c->ny != 0)
		{
			min_ty = c->t;
			ny = c->ny;
			min_iy = i;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}

void GameObject::update(const DWORD dt, vector<GameObject*>* coObject)
{
	this->dt = dt;
	createBlowUpEffectAndSetRespawnTimer();
	processUntouchableEffect();
	processWhenBurnedEffectDone();
	dx = vx * dt;
	dy = vy * dt;
}

void GameObject::processDeathEffect()
{
	if (isDying())
	{
		animId = ANIM_DEATH;
		setEnable(false);
		startDying = true;
	}
	else if (startDying)
	{
		startDying = false;
	}
}

void GameObject::processUntouchableEffect()
{
	if (isUntouching()) alpha = rand() % 255;
	else alpha = 255;
}

void GameObject::processDeflectEffect()
{
	if (isDeflecting())
	{
		vx = vxDeflect * nxDeflect;
		vy = -vyDeflect;
		startDeflect = true;
		faceSide = -nxDeflect;
	}
	else if (startDeflect)
	{
		doUntouchable();
		startDeflect = false;
		x -= nxDeflect * 0.01f; // case that collide boundary need more space
	}
}

void GameObject::createBlowUpEffectAndSetRespawnTimer()
{
	if (state == death)
	{
		if (!burnEffect && !timerBurnEffect->runAlready())
		{
			const auto now = GetTickCount();
			burnEffect = AnimationManager::getInstance()->get(ANIM_BURNED);
			burnEffect->setAniStartTime(now);
			setEnable(false);
			timerBurnEffect->start();
		}
	}
}

void GameObject::checkCollisionAndStopMovement(DWORD dt, vector<GameObject*>* coObjects)
{
	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;
	coEvents.clear();

	calcPotentialCollisions(coObjects, coEvents);
	// no collison
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
	}

	for (auto& coEvent : coEvents) delete coEvent;
}

void GameObject::updatePosWhenNotCollide()
{
	x += dx;
	y += dy;
}

void GameObject::updateGravity(float gravity)
{
	vy += gravity * dt;
	this->gravity = gravity;
}

void GameObject::updatePosInTheMomentCollide(float minTx, float minTy, float nx, float ny)
{
	x += minTx * dx + nx * 0.4f;
	y += minTy * dy + ny * 0.4f;
	if (nx != 0) vx = 0;
	if (ny != 0) vy = 0;
}

Box GameObject::getBoundingBoxBaseOnFile()
{
	float r, l;
	auto spriteFrame = animations[animId]->getFrameSprite();
	auto spriteBoundary = animations[animId]->getFrameBoundingBox();
	auto offset = getOffsetFromBoundingBox();

	if (faceSide == right)
	{
		r = x + (spriteFrame.r - spriteFrame.l) - offset.x;
		l = r - (spriteBoundary.r - spriteBoundary.l);
	}
	else
	{
		l = x + offset.x;
		r = spriteBoundary.r - spriteBoundary.l + l;
	}

	const float t = y + offset.y;
	const float b = t + (spriteBoundary.b - spriteBoundary.t);

	// neu truyen width vao tinh left right o giua enemy
	return Box(l, t, r, b);
}

Box GameObject::getBoundingBoxBaseOnFileAndPassWidth(float width)
{
	const auto box = getBoundingBoxBaseOnFile();
	const float bboxWidth = box.r - box.l;
	const float l = x + bboxWidth / 2 - width / 2;
	const float r = l + width;
	return Box(l, box.t, r, box.b);
}

D3DXVECTOR2 GameObject::getOffsetFromBoundingBox()
{
	auto spriteFrame = animations[animId]->getFrameSprite();
	auto spriteBoundary = animations[animId]->getFrameBoundingBox();

	// spriteFrame is usually larger than the spriteBoundary so we need to take account of the offset
	auto offsetX = spriteBoundary.l - spriteFrame.l;
	auto offsetY = spriteBoundary.t - spriteFrame.t;
	return { offsetX, offsetY };
}