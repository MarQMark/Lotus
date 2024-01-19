#ifndef LOTUS_CONSTANTS_H
#define LOTUS_CONSTANTS_H

enum Nation{
    FIRE = 0,
    EARTH = 1,
    AIR = 2,
    WATER = 3
};

static float GRAVITY = -.004f;

static float PLAYER_FRICTION = 0.05f;
static float MOVEMENT_SPEED = 0.035f;
static float JUMP_FORCE = 1.1f;

static float PLAYER_WIDTH = 50;
static float PLAYER_HEIGHT = 80;
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
static double ULT_COOLDOWN[4] = {
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
static double ULT_CAST[4] = {
        200.f,
        200.f,
        200.f,
        200.f
};
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
        10.,
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
static float ULT_GAIN[4] = {
        0.01f,
        0.01f,
        0.01f,
        0.01f
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
