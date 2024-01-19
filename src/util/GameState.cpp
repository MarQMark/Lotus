#include <unordered_map>
#include "Constants.h"
#include "Kikan/ecs/Entity.h"
#include "util/GameState.h"
#include "components/PlayerStateComponent.h"


GameState& GameState::getInstance() {
    static GameState instance;
    return instance;
}

void GameState::clearPlayerInput(unsigned int playerSignature)
{
    playerInputs[playerSignature] = 0;
}

void GameState::setPlayerInput(unsigned int playerSignature, unsigned int input) {
    if(input == 0)
        playerInputs[playerSignature] = input;
    else
        playerInputs[playerSignature] |= input;
}

void GameState::setDirecInput(unsigned int playerSignature, unsigned int input)
{
    playerInputs[playerSignature] = input;
}

unsigned int  GameState::getPlayerInput(unsigned int playerSignature) const {
    auto it = playerInputs.find(playerSignature);
    return (it != playerInputs.end()) ? it->second : 0;
}

void GameState::setPlayerComponent(unsigned int playerSignature) {
    playerComponents[playerSignature] = playerSignature;
}

unsigned int GameState::getPlayerComponent(unsigned int playerSignature) const {
    auto it = playerComponents.find(playerSignature);
    return (it != playerComponents.end()) ? it->second : 0;
}

void GameState::setPlayerInputHistory(unsigned int playerSignature, int frameCount,  unsigned int  input)
{
        if (frameCount >= 0 && frameCount < INPUT_HISTOTY_LENTGH) {
            // Set the input value for the specified player and frame
            inputHistory[playerSignature][frameCount] = input;

        }
}

unsigned int GameState::getPlayerInputHistory(unsigned int playerSignature, int frameCount) const
{
    auto playerIt = inputHistory.find(playerSignature);
    return (playerIt != inputHistory.end()) ? playerIt->second[frameCount] : 0;
}


unsigned int GameState::getEnemyId() {
    return 0;
}

unsigned int GameState::getPlayerId() {
    return 0;
}
