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

static float PLAYER_WIDTH = 50;
static float PLAYER_HEIGHT = 80;

// ----------------------------- FIRE -----------------------------
static float FIRE_ATTACK_VEL = 1.f;
static double FIRE_ATTACK_COOL = 1000.f; // in millis
static double FIRE_ATTACK_CAST = 200.f; // in millis

static double FIRE_MOV_ANI_SPEED = 100;

static double FIRE_ATK_ANI_SPEED = 100;

static uint32_t FIRE_ATK_SPRITE_HEIGHT = 30;
static double FIRE_ATK_DAMAGE = 10;

// ---------------------------- EARTH ----------------------------
static float EARTH_ATTACK_VEL = 1.f;

static double EARTH_MOV_ANI_SPEED = 100;
static double EARTH_ATTACK_ANI_SPEED = 100;

static double EARTH_ATK_ANI_SPEED = 100;

static uint32_t EARTH_ATK_SPRITE_HEIGHT = 20;
static double EARTH_ATK_DAMAGE = 10;

// ----------------------------- AIR ------------------------------
static float AIR_ATTACK_VEL = 1.f;

static double AIR_MOV_ANI_SPEED = 100;
static double AIR_ATTACK_ANI_SPEED = 100;

static double AIR_ATK_ANI_SPEED = 100;

static uint32_t AIR_ATK_SPRITE_HEIGHT = 50;
static double AIR_ATK_DAMAGE = 10;

// ---------------------------- WATER ----------------------------
static float WATER_ATTACK_VEL = 1.f;

static double WATER_MOV_ANI_SPEED = 100;
static double WATER_ATTACK_ANI_SPEED = 100;

static uint32_t WATER_ATK_SPRITE_HEIGHT = 10;
static double WATER_ATK_DAMAGE = 10;

static double WATER_ATK_ANI_SPEED = 100;

#endif //LOTUS_CONSTANTS_H
