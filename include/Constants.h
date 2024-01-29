#ifndef LOTUS_CONSTANTS_H
#define LOTUS_CONSTANTS_H

#include <cstdint>

constexpr int NET_INPUT_DELAY = 1;

constexpr int INPUT_HISTOTY_LENTGH = 3*60*60;

constexpr  unsigned int NET_PACKET_INPUT_HISTORY_SIZE = 10;

struct NetworkInputPackage
{
    unsigned int InputHistory[NET_PACKET_INPUT_HISTORY_SIZE];
    int FrameCount { 0 };
};


enum Nation{
    FIRE = 0,
    EARTH = 1,
    AIR = 2,
#ifdef ENABLE_WATER
    WATER = 3
#endif
};

enum class InputCommand : unsigned int
{
    None = 0,
    Left = 1,
    Right = 2,
    Jump = 4,
    Ability = 8,
    Attack = 16,
    Super = 32,
};

enum class NetworkState
{
    None,
    Hosting,
    Client,
};

static float GRAVITY = -.004f;

static float PLAYER_FRICTION = 0.05f;
static float MOVEMENT_SPEED = 0.035f;
static float JUMP_FORCE = 1.1f;

static float PLAYER_WIDTH = 40;
static float PLAYER_HEIGHT = 70;
static double PLAYER_HEALTH = 100;


static double ATTACK_COOLDOWN[4] = {
        1000.,
        1000.,
        1000.,
        1000.
};
static double ABILITY_COOLDOWN[4] = {
        2000.,
        2000.,
        2000.,
        2000.
};
static float ATTACK_VEL[4] =  {
        0.1f,
        1.f,
        1.f,
        1.f
};
static double ATTACK_CAST[4] = {
        200.f,
        200.f,
        200.f,
        200.f
};
static double ABILITY_CAST[4] = {
        200.f,
        200.f,
        200.f,
        2000.f
};
#ifdef ENABLE_ULT
static double ULT_CAST[4] = {
        200.f,
        200.f,
        200.f,
        200.f
};
static double ULT_COOLDOWN[4] = {
        2000.,
        2000.,
        2000.,
        2000.
};
static float ULT_GAIN[4] = {
        0.01f,
        0.01f,
        0.01f,
        0.01f
};
#endif
static double MOVE_ANI_SPEED[4] = {
        100.,
        100.,
        100.,
        100.
};
static double ATK_ANI_SPEED[4] = {
        100.,
        100.,
        100.,
        100.
};
static double ATK_DAMAGE[4] = {
        70.,
        10.,
        10.,
        10.
};
static uint32_t ATK_SPRITE_HEIGHT[4] = {
        30,
        20,
        50,
        10
};

// ----------------------------- FIRE -----------------------------
// ---------------------------- EARTH ----------------------------
static float EARTH_ABL_SPRITE_WIDTH = 100;
static double EARTH_ABL_DAMAGE = 30;
static float EARTH_ABL_IMPULSE = .1;

// ----------------------------- AIR ------------------------------
// ---------------------------- WATER ----------------------------
static double HEAL_HEALTH = 20;

#endif //LOTUS_CONSTANTS_H
