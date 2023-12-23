#ifndef LOTUS_CONSTANTS_H
#define LOTUS_CONSTANTS_H

enum Nation{
    FIRE,
    WATER,
    EARTH,
    AIR
};

static float GRAVITY = -.004f;

static float PLAYER_FRICTION = 0.05f;
static float MOVEMENT_SPEED = 0.035f;
static float JUMP_FORCE = 1.1f;

// ----------------------------- FIRE -----------------------------
static float FIRE_ATTACK_VEL = 1.f;
static double FIRE_ATTACK_COOL = 1000.f; // in millis

static double FIRE_MOV_ANI_SPEED = 100;

// ---------------------------- EARTH ----------------------------
static double EARTH_MOV_ANI_SPEED = 100;
static double EARTH_ATTACK_ANI_SPEED = 100;

// ----------------------------- AIR ------------------------------
static double AIR_MOV_ANI_SPEED = 100;
static double AIR_ATTACK_ANI_SPEED = 100;

// ---------------------------- WATER ----------------------------
static double WATER_MOV_ANI_SPEED = 100;
static double WATER_ATTACK_ANI_SPEED = 100;

#endif //LOTUS_CONSTANTS_H
