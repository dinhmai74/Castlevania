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
constexpr auto HEADER_HEIGHT = 80;
constexpr auto TEXT_COLOR = D3DCOLOR_XRGB(252, 252, 252);
constexpr auto DEFAULT_TIME_PLAY = 300;
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
	staring = 9,
	deflect = 99,
	death = -1,
	normal = 1,
};

enum CandleType
{
	candleBig,
	candleSmall
};

enum ObjectType
{
	boundary,
	item,
	candle,
	canHitObjs,
	subWeapon,
	whip,
	simon,
	obChangeStage,
};

enum BoundaryType {
	boundaryNormal,
};

/*----------------- item regions -----------------*/
#pragma region Items

auto constexpr G_HEART_SMALL = 0.0001f;

enum ItemType
{
	itemDagger,
	itemAxe,
	itemBoomerang,
	itemHolyWater,
	itemRedMoneyBag,
	itemWhiteMoneyBag,
	itemBlueMoneyBag,
	itemWhip = 102,
	itemBigHeart = 100,
	itemSmallHeart = 101,
	itemHolyWaterEffect = 103,
};

#pragma endregion

/*----------------- candle -----------------*/
auto constexpr CANDLE_DESTROYED_TIME = 300;

/*
	texture constants
*/
constexpr auto ID_TEX_BBOX = -101; //
constexpr auto ID_TEX_BLACK_BOARD = 1000001; //

/*----------------- normal anim -----------------*/
auto constexpr ANIM_BURNED = "burned_effect_default_ani";
auto constexpr BURNED_DURATION = 400;

/*----------------- Timer  -----------------*/
auto constexpr TIMER_ITEM_DURATION = 5000;
auto constexpr TIMER_ITEM_UNTOUCHABLE_DURATION = 2000;
auto constexpr TIMER_HOLY_BURN_DURATION = 2000;
