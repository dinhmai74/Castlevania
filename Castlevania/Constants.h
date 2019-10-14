#pragma once
#include "StageConst.h"
#include "WhipConst.h"
#include "TileConst.h"
#include "Box.h"
#include <d3d9.h>
#include <d3dx9.h>
using namespace std;

enum Side
{
	SideLeft = -1,
	SideRight = 1
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
constexpr auto CDIR_LEFT = -1.0f;
constexpr auto CDIR_RIGHT = 1.0f;
constexpr auto CDIR_TOP = 1.0f;
constexpr auto CDIR_BOTTOM = -1.0f;

//states
enum State
{
	idle ,
	walking ,
	sitting ,
	jumping ,
	hitting ,
	hittingWhenSitting ,
	hittingWhenClimbing,
	throwing,
	throwingWhenSitting,
	throwingWhenClimbing,
	climbing,
	disabled,
	deflect = 99,
	death = -1,
	normal = 0,
};

enum ObjectType
{
	OBBoundary,
	OBItem,
	OBCandle,
	OBCanHitObjs,
	temp1,
	OBwhip,
	temp0,
	OBChangeStage,
	OBEnemy,
	OBStair,
	OBDoor,
	OBForceIdleSim,
	OBSimon = 50,
	OBCastle = 51,
	OBSubWeapon = 52,
};

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


/*----------------- gravity  -----------------*/
auto constexpr E_GRAVITY = 0.0015f;
auto constexpr G_HEART_SMALL = 0.0001f;

struct VectorInt
{
	int x;
	int y;
};
