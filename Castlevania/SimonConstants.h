#pragma once

constexpr auto SIMON_DEAD_TIME = 3000;
//nhay
constexpr auto SIMON_VJUMP = 0.5f;
constexpr auto SIM_WIDTH = 32;
constexpr auto SIM_VY_READY_TO_LAND = 0.15f;

/*----------------- time  -----------------*/
constexpr auto SIM_HITTING_TIME = 450;
constexpr auto SIM_POWERING_DURATION = 1000;
constexpr auto SIM_DELTA_TRHOWING_TIME = 200;
constexpr auto SIM_AUTO_WALK_DURATION = 1000;
constexpr auto SIM_SIT_WHEN_LANDING = 100;
constexpr auto SIM_CHANGING_STAGE_DURATION = 1000;
constexpr auto SIM_ADD_HP_DURATION= 100;
constexpr auto SIM_EXTRA_SHOT_DURATION= 1000;
constexpr auto SIM_ITEM_GOLD_POTION_DURATION = 6000;


// trong luc luc nhay
constexpr auto SIMON_GRAVITY = 0.0015f;
//di binh thuong
constexpr auto SIMON_VX = 0.10f;
constexpr auto SIM_AUTO_WALK_DISTANCE_VX = 0.15f;
constexpr auto SIM_AUTO_WALK_VX = 0.03f;
constexpr auto SIM_CLIMB_VELOCITY = 0.075;
constexpr auto SIM_MAX_ENERGY = 15;
constexpr auto SIM_MAX_LIFE = 15;
constexpr auto SIM_MAX_HP = 15;

/*
 * simon animation constants
 */
constexpr auto ANIM_WALK = 1;
constexpr auto ANIM_IDLE = 0;
constexpr auto ANIM_SIT = 2;
constexpr auto ANIM_HITTING = 3;
constexpr auto ANIM_HITTING_WHEN_SIT = 4;
constexpr auto ANIM_DEFLECT = 100;
constexpr auto ANIM_UP_STAIR = 6;
constexpr auto ANIM_DOWN_STAIR = 7;
constexpr auto ANIM_HIT_DOWN_STAIR = 8;
constexpr auto ANIM_HIT_UP_STAIR = 9;
constexpr auto ANIM_IDLE_BACK= 10;
constexpr auto ANIM_DEATH = 101;

enum ClimbDirect {
	ClimbNull,
	ClimbUp = 1,
	ClimbDown = -1,
};

enum StaringStatus {
	none,
	pause,
	ready,
	onGoing
};

enum ThroughDoorStatus {
	nope,
	ThroughDoorStarted,
	ThroughDoorOpening,
	ThroughStartWalk,
	ThroughWalkDone,
	ThroughDoorStartClose,
	ThroughDoorClosing,
	ThroughDoorClosed,
	ThroughDone
};