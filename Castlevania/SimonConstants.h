#pragma once

constexpr auto SIMON_DEAD_TIME = 3000;
//nhay
constexpr auto SIMON_VJUMP = 0.5f;
// Simon's width when moving
constexpr auto SIM_MOVE_W = 32;
// Simon's width when moving
constexpr auto SIM_MOVE_H = 62;
// Simon's width when hitting (normal state)
constexpr auto SIM_HIT_W = 48;
// Simon's height when hitting (normal state)
constexpr auto SIM_HIT_H = 62;
// Simon's width when sitting
constexpr auto SIM_SIT_W = 32;
// Simon's height when sitting
constexpr auto SIM_SIT_H = 48;
// Simon's width when hitting( in sitting state)
constexpr auto SIM_HIT_WHEN_SIT_W = 48;
// Simon's height when hitting( in sitting state)
constexpr auto SIM_HIT_WHEN_SIT_H = 48;
constexpr auto SIM_VY_READY_TO_LAND = 0.15f;

/*----------------- time  -----------------*/
constexpr auto SIM_HITTING_TIME = 100;
constexpr auto SIM_JUMPING_TIME= 300;
constexpr auto SIM_THROWING_TIME = 150;
constexpr auto SIM_POWERING_DURATION= 1000;
constexpr auto SIM_DELTA_TRHOWING_TIME= 400;
constexpr auto SIM_AUTO_WALK_DURATION= 1000;
constexpr auto SIM_CHANGING_STAGE_DURATION= 2000;

// trong luc luc nhay
#define SIMON_GRAVITY 0.0015f;
//trong luc luc roi
constexpr auto SIMON_FALL = 180;
//di binh thuong
constexpr auto SIMON_VX = 0.10f;
constexpr auto SIM_AUTO_WALK_VX = 0.03f;
constexpr auto MAX_ENERGY= 15;
//len xuong cau thang
#define SIMON_VX_STAIR 43	
#define SIMON_VY_STAIR 43


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