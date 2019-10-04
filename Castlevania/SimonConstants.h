#pragma once

constexpr auto SIMON_DEAD_TIME = 3000;
//nhay
constexpr auto SIMON_VJUMP = 0.5f;
constexpr auto SIM_WIDTH = 32;
constexpr auto SIM_VY_READY_TO_LAND = 0.15f;

/*----------------- time  -----------------*/
constexpr auto SIM_HITTING_TIME = 100;
constexpr auto SIM_THROWING_TIME = 150;
constexpr auto SIM_POWERING_DURATION = 1000;
constexpr auto SIM_DELTA_TRHOWING_TIME = 400;
constexpr auto SIM_AUTO_WALK_DURATION = 1000;
constexpr auto SIM_CHANGING_STAGE_DURATION = 2000;

// trong luc luc nhay
#define SIMON_GRAVITY 0.0015f;
//trong luc luc roi
constexpr auto SIMON_FALL = 180;
//di binh thuong
constexpr auto SIMON_VX = 0.10f;
#define SIMON_STAIR_SPEED_X			0.079f
#define SIMON_STAIR_SPEED_Y			0.079f
constexpr auto SIM_AUTO_WALK_VX = 0.03f;
constexpr auto SIM_MAX_ENERGY = 15;
constexpr auto SIM_MAX_LIFE = 15;
constexpr auto SIM_MAX_HP = 15;
//len xuong cau thang
#define SIMON_VX_STAIR 43
#define SIMON_VY_STAIR 43

/*
 * simon animation constants
 */
constexpr auto ANIM_WALK = 1;
constexpr auto ANIM_IDLE = 0;
constexpr auto ANIM_SIT = 2;
constexpr auto ANIM_HITTING = 3;
constexpr auto ANIM_HITTING_WHEN_SIT = 4;
constexpr auto ANIM_DEFLECT = 5;
constexpr auto ANIM_UP_STAIR = 6;
constexpr auto ANIM_DEATH = -1;

enum StaringDirect
{
	stairNull,
	stairUp=-1,
	stairDown=1,
};

enum StaringStatus
{
	pause,
	ready,
	onGoing
};