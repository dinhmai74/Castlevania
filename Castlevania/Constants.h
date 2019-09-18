#pragma once
#include "SimonConstants.h"
#include "StageConst.h"
#include "WhipConst.h"
#include "TileConst.h"
#include "EnvConst.h"
#include "ItemConst.h"
#include "Box.h"
#include <d3d9.h>
#include <d3dx9.h>
using namespace std;

// huong mat. object dang nhin
enum FaceSide {
	left = -1, right = 1
};

constexpr auto BRICK_2_SIZE = 32;
constexpr auto LV1_GROUND_Y = 384;
constexpr auto HEADER_HEIGHT = 60;
// Orientation
constexpr auto  DIR_LEFT = -1;
constexpr auto  DIR_RIGHT = 1;
// Collision direction
constexpr auto  CDIR_LEFT = 1.0f;
constexpr auto  CDIR_RIGHT = -1.0f;
constexpr auto  CDIR_TOP = 1.0f;
constexpr auto 	CDIR_BOTTOM = -1.0f;
constexpr auto MAIN_TEXTURE_TRANS_COLOR = D3DCOLOR_XRGB(255, 0, 255);
constexpr auto  TILEMAP_TRANSPARENT_COLOR = D3DCOLOR_XRGB(5, 5, 5);
constexpr auto  BOUND_BBOX_COLOR = D3DCOLOR_XRGB(255, 163, 177);

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
	normal = 1
};

/*
	texture constants
*/

constexpr auto ID_TEX_SIMON = 101;
constexpr auto ID_TEX_EMPTY = 701;
constexpr auto ID_TEX_BIG_CANDLE = 502;		//
constexpr auto ID_TEX_WHIP = 301;
constexpr auto ID_TEX_DAGGER= 401;
constexpr auto ID_TEX_BBOX = -101;		//
constexpr auto ID_TEX_BRICK_2 = 201;		//
constexpr auto ID_TEX_HEART_ITEM = 601;		//
constexpr auto ID_TEX_WHIP_ITEM = 602;
constexpr auto ID_TEX_KNIFE_ITEM = 603;
constexpr auto ID_TEX_BURN_EFFECT = 801;
constexpr auto ID_TEX_COLLISION_EFFECT = 802;

constexpr auto ID_TEX_BACKGROUND_LV1 = 1;		//
constexpr auto ID_TEX_MAP_LV1= 10000;		//
constexpr auto ID_TEX_ENTRANCE_BACKGROUND = 2;		//


/* Animation constants */

constexpr auto ANIMATION_BRICK_IDLE = 201;

/*
 * simon animation constants
 */
constexpr auto ANIM_SIM_WALKING_R = 101;
constexpr auto ANIM_WALK = 1;
constexpr auto ANIM_IDLE = 0;
constexpr auto ANIM_SIT = 113;
constexpr auto ANIM_HITTING = 121;
constexpr auto ANIM_HITTING_WHEN_SIT = 124;
constexpr auto ANIM_SIM_THROW_L = 125;
constexpr auto ANIM_SIM_THROW_R = 126;
constexpr auto ANIM_SIM_THROW_WHEN_SIT_L = 127;
constexpr auto ANIM_SIM_THROW_WHEN_SIT_R = 128;

/*
 * whip animation constants
 */
constexpr auto ANIM_WHIP_LV1_L = 301;
constexpr auto ANIM_WHIP_LV1_R = 302;
constexpr auto ANIM_WHIP_LV2_L = 303;
constexpr auto ANIM_WHIP_LV2_R = 304;
constexpr auto ANIM_WHIP_LV3_L = 305;
constexpr auto ANIM_WHIP_LV3_R = 306;
constexpr auto ANIM_DAGGER_L= 407;
constexpr auto ANIM_DAGGER_R= 408;

/*
 * effect ani constants
 */
constexpr auto ANIM_BURN_EFFECT = 801;
constexpr auto ANIM_COLLISION_EFFECT= 802;

/*
 * item animation constants
 */
constexpr auto ANIM_BIG_CANDLE_IDLE = 501;
constexpr auto ANIM_HEART_ITEM_IDLE = 601;
constexpr auto ANIM_WHIP_ITEM_IDLE = 602;
constexpr auto ANIM_DAGGER_ITEM_IDLE = 603;
