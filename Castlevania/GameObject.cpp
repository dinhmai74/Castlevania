#include "GameObject.h"
#include <algorithm>


GameObject::GameObject()
{
	x = y = 0;
	initPos = { 0,0 };
	vx = vy = 0;
	faceSide = FaceSide::right; // right side
	previousAmiId = -1;
	previousAnimIsOneTimeAnim = false;
	boundingGameX = 0;
	boundingGameY = 0;
	currentState = State::normal;
	gravity = 0;
}

GameObject::~GameObject()
{
	if (texture != nullptr) texture->Release();
}

void GameObject::render()
{
}


void GameObject::renderBoundingBox()
{
	const auto texture = TextureManager::getInstance()->get(ID_TEX_BBOX);
	auto game = Game::getInstance();
	const auto rect = getBoundingBox();

	game->draw(faceSide, rect.left, rect.top, texture, rect, rect, 140);
}

void GameObject::addAnimation(int id, string animTexId)
{
	auto animation = AnimationManager::getInstance()->get(animTexId);
	animations[id] = animation;
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

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i;
		}

		if (c->t < min_ty && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}


void GameObject::update(const DWORD dt, vector<GameObject*>* coObject)
{
	this->dt = dt;
	dx = vx * dt;
	dy = vy * dt;
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

		if (ny == -1.0f)
		{
			// nếu va chạm với đất set vy=0 để đỡ bug va chạm. 
			// k set vy=0 ( có lúc rớt xuống trùng với đất nhanh quá sweepAABB k xét va chạm)
			vy = 0;
		}
	}

	for (auto& coEvent : coEvents) delete coEvent;
}

void GameObject::updatePosWhenNotCollide()
{
	x += dx;
	y += dy;
	updateGravity(gravity);
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
}


Box GameObject::getBoundingBoxBaseOnFile()
{
	float r, l;
	if (!animations[animationId]) return{ x,y,1,1 };
	auto spriteFrame = animations[animationId]->getFrameSprite();
	auto spriteBoundary = animations[animationId]->getFrameBoundingBox();

	// spriteFrame is usually larger than the spriteBoundary so we need to take account of the offset
	auto offset_x = spriteBoundary.left - spriteFrame.left;
	auto offset_y = spriteBoundary.top - spriteFrame.top;

	if (faceSide == FaceSide::right)
	{
		r = x + (spriteFrame.right - spriteFrame.left) - offset_x;
		l = r - (spriteBoundary.right - spriteBoundary.left);
	}
	else
	{
		l = x + offset_x;
		r = spriteBoundary.right - spriteBoundary.left + l;
	}

	const auto t = y + offset_y;
	const auto b = t + (spriteBoundary.bottom - spriteBoundary.top);

	// neu truyen width vao tinh left right o giua enemy

	return { l,t,r,b };
}

Box GameObject::getBoundingBoxBaseOnFileAndPassWidth(float width)
{
	const auto box = getBoundingBoxBaseOnFile();
	const auto bboxWidth = box.right - box.left;
	const auto l = x + bboxWidth / 2 - width / 2;
	const auto r = l + width;
	return { (l), box.top, (r), box.bottom };
}


