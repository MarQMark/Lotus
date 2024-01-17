
#ifndef LOTUS_GAMESTATE_H
#define LOTUS_GAMESTATE_H
#include <unordered_map>
#include "Constants.h"
#include "components/PlayerStateComponent.h"
#include "Kikan/ecs/Entity.h"

class GameState {
public:
    static GameState& getInstance();

    void setDirecInput(unsigned int playerSignature, unsigned int input);

    // Function to clear player input for a specific player entity
    void clearPlayerInput(unsigned int playerSignature);

    // Function to set player input for a specific player entity
    void setPlayerInput(unsigned int playerSignature,  unsigned int  input);

    // Function to get player input for a specific player entity
    unsigned int getPlayerInput(unsigned int playerSignature) const;

    // Function to set player components for a specific player entity
    void setPlayerComponent(unsigned int playerSignature);

    // Function to get player components for a specific player entity
    unsigned int getPlayerComponent(unsigned int playerSignature) const;

    unsigned int getEnemyId();

    unsigned int getPlayerId();

    int FrameCount { 0 };
    int NetFrameCount { 0 };

    bool UpdateGame = false;

private:
    std::unordered_map<unsigned int, unsigned int> playerInputs;
    std::unordered_map<unsigned int, unsigned int> playerComponents;


    // Private constructor to prevent instantiation
    GameState() = default;
    ~GameState() = default;
    GameState(const GameState&) = delete;
    GameState& operator=(const GameState&) = delete;
};

#endif //LOTUS_GAMESTATE_H
