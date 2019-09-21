#pragma once
#include "SimonConstants.h"
#include "StageConst.h"
#include "WhipConst.h"
#include "TileConst.h"
#include "Box.h"
#include <d3d9.h>
#include <d3dx9.h>
using namespace std;

enum FaceSide
{
	left = -1,
	right = 1
};

/*----------------- general  -----------------*/
constexpr auto MAIN_TEXTURE_TRANS_COLOR = D3DCOLOR_XRGB(255, 0, 255);
constexpr auto TILEMAP_TRANSPARENT_COLOR = D3DCOLOR_XRGB(5, 5, 5);
constexpr auto BOUND_BBOX_COLOR = D3DCOLOR_XRGB(255, 163, 177);
constexpr auto HEADER_HEIGHT = 60;
// Orientation
constexpr auto DIR_LEFT = -1;
constexpr auto DIR_RIGHT = 1;
// Collision direction
constexpr auto CDIR_LEFT = 1.0f;
constexpr auto CDIR_RIGHT = -1.0f;
constexpr auto CDIR_TOP = 1.0f;
constexpr auto CDIR_BOTTOM = -1.0f;

//states
enum State
{
	idle = 1,
	walking = 2,
	sitting = 3,
	jumping = 4,
	hitting = 5,
	hittingWhenSitting = 6,
	throwing = 7,
	throwingWhenSitting = 8,
	dead = -1,
	normal = 1,
};

enum CandleType
{
	bigCandle,
	smallCandle
};

enum ObjectType
{
	boundary,
	item,
	candle,
	canHitObjs,
	subWeapon,
	whip,
	simon
};


/*----------------- item regions -----------------*/
#pragma region Items

auto constexpr G_HEART_SMALL = 0.0001f;

enum ItemType
{
	itemSmallHeart,
	itemBigHeart,
	itemDagger,
	itemWhip,
	itemRedMoneyBag,
	itemWhiteMoneyBag,
	itemBlueMoneyBag,
	itemCross,
	itemAxe,
};

#pragma endregion

/*----------------- candle -----------------*/
auto constexpr CANDLE_DESTROYED_TIME = 300;

/*
	texture constants
*/
constexpr auto ID_TEX_BBOX = -101; //

/*----------------- normal anim -----------------*/
auto constexpr ANIM_BURNED = "burned_effect_default_ani";
auto constexpr BURNED_DURATION= 400;
