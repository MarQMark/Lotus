#ifndef LOTUS_MESSAGES_H
#define LOTUS_MESSAGES_H

#include <cinttypes>

#define MESSAGE_SIZE(x) sizeof(Header) + x.hdr.len

enum MessageID {
    C2SAuth,
    S2CAuth,
    S2CJoin,
    C2SPos,
    S2CData
};

/*
 * Header for all messages
 *
 * ID: type of message contained in the body
 * Len: Size of body
 */
typedef struct __attribute__((packed)) {
    uint16_t id;
    uint16_t len;
}Header;

/*
 * TODO: Add some kind of authentication
 *
 * Dummy data cause you cant receive an empty body with
 */
typedef struct __attribute__((packed)) {
    uint16_t dummy;
}C2S_Auth;

/*
 * contains a unique id for the player
 * This will be used to identify the player
 */
typedef struct __attribute__((packed)) {
    uint16_t playerID;
}S2C_Auth;

/*
 * Send all existing players new player
 */
typedef struct __attribute__((packed)) {
    uint16_t playerID;
}S2C_Join;

/*
 * Client tells server its position
 *
 * later this will include more info
 */
typedef struct __attribute__((packed)) {
    float x;
    float y;
}C2S_Pos;

typedef union __attribute__((packed)) {
    C2S_Auth c2s_auth;
    S2C_Auth s2c_auth;
    S2C_Join s2c_join;
    C2S_Pos c2s_pos;
}Body;

typedef struct __attribute__((packed)) {
    Header hdr;
    Body body;
}Message;


/*
 * Default data every player receives from other players
 */
typedef struct __attribute__((packed)) {
    uint16_t id;
    float x;
    float y;
}PlayerData;
/*
 * Separate struct since union size has to be known at compile time
 * and number of players can vary.
 */
typedef struct __attribute__((packed)) {
    Header hdr;
    PlayerData data[];
}DataMessage;

#endif //LOTUS_MESSAGES_H
