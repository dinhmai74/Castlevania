#pragma once

constexpr auto SIMON_DEAD_TIME = 3000;



//nhay
constexpr auto SIMON_VJUMP = 0.35f;
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

constexpr auto SIMON_HITTING_TIME = 100;
constexpr auto SIMON_JUMPING_TIME= 300;
constexpr auto SIMON_THROWING_TIME = 150;

// trong luc luc nhay
#define SIMON_GRAVITY 0.0011f;
//trong luc luc roi
constexpr auto SIMON_FALL = 180;
//di binh thuong
constexpr auto SIMON_VX = 0.17f;;
//len xuong cau thang
#define SIMON_VX_STAIR 43	
#define SIMON_VY_STAIR 43