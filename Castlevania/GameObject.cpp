#include "GameObject.h"
#include <algorithm>


GameObject::GameObject()
{
	x = y = 0;
	vx = vy = 0;
	faceSide = FaceSide::right; // right side
	previousAmiId = -1;
	previousAnimIsOneTimeAnim = false;
	boundingGameX = 0;
	boundingGameY = 0;
	currentState = State::normal;
}

GameObject::~GameObject()
{
	if (texture != nullptr) texture->Release();
}


void GameObject::renderBoundingBox()
{
	const auto texture = TextureManager::getInstance()->get(ID_TEX_BBOX);
	auto game = Game::getInstance();
	const auto rect = getBoundingBox();

	game->draw(faceSide,rect.left, rect.top,texture,rect, rect, 40);
}

void GameObject::addAnimation(int id, string animTexId)
{
	auto animation = AnimationManager::getInstance()->get(animTexId);
	animations[id] = animation;
}


LPCollisionEvent GameObject::sweptAABBEx(LPGAMEOBJECT coO)
{
	float t, nx, ny;

	auto coBox=coO->getBoundingBox();

	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	coO->getSpeed(svx, svy);

	float sdx = svx * dt;
	float sdy = svy * dt;

	float dx = this->dx - sdx;
	float dy = this->dy - sdy;

	auto obBox=getBoundingBox();

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
	for (auto& coObject : *coObjects)
	{
		LPCollisionEvent e = sweptAABBEx(coObject);

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
	float & min_tx, float & min_ty, float & nx, float & ny)
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

		if (c->t < min_ty  && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}


void GameObject::update(const DWORD dt, vector<GameObject*> *coObject)
{
	this->dt = dt;
	dx = vx * dt;
	dy = vy * dt;
}

RECT GameObject::getBoundingBoxBaseOnFile()
{
	LONG r, l;
	if (!animations[animationId]) return {static_cast<LONG>(x), static_cast<LONG>(y),1,1 };
	auto spriteFrame = animations[animationId]->getFrameRect();
	auto spriteBoundary = animations[animationId]->getFrameBBoxRect();

	// spriteFrame is usually larger than the spriteBoundary so we need to take account of the offset
	auto offset_x = spriteBoundary.left - spriteFrame.left;
	auto offset_y = spriteBoundary.top - spriteFrame.top;

	if (faceSide== FaceSide::right)
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

	return {static_cast<LONG>(l), static_cast<LONG>(t), static_cast<LONG>(r), static_cast<LONG>(b)};
}

RECT GameObject::getBoundingBoxBaseOnFileAndPassWidth(float width)
{
	const auto box = getBoundingBoxBaseOnFile();
	const auto bboxWidth = box.right - box.left;
	const auto l = x + bboxWidth / 2 - width / 2;
	const auto r = l + width;
	return {static_cast<LONG>(l), box.top, static_cast<LONG>(r), box.bottom };
}


