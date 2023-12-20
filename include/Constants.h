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


static float FIRE_ATTACK_VEL = 1.f;
static double FIRE_ATTACK_COOL = 1000.f; // in millis

static double FIRE_MOV_ANI_SPEED = 100;

#endif //LOTUS_CONSTANTS_H
