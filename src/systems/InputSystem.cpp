#include "systems/InputSystem.h"
#include "components/PlayerComponent.h"
#include "Kikan/ecs/Entity.h"
#include "Kikan/Engine.h"
#include "Kikan/input/Input.h"
#include "Kikan/ecs/components/Physics.h"
#include "components/DColliderComponent.h"
#include "Constants.h"
#include "util/Spawner.h"
#include "components/TriggerComponent.h"
#include "components/EffectComponent.h"
#include "components/MessageComponent.h"
#include "components/PlayerStateComponent.h"
#include "util/GameState.h"

unsigned int NetInputBuffer = 0;


InputSystem::InputSystem() {
    includeSingle(PlayerComponent);
}

void InputSystem::update(double dt) {
    GameState& gameState = GameState::getInstance();
    for (Kikan::Entity *e: _entities) {
        auto *transform = e->getComponent<Kikan::Transform>();
        auto *physics = e->getComponent<Kikan::Physics>();
        auto *collider = e->getComponent<DColliderComponent>();
        auto *player = e->getComponent<PlayerStateComponent>();
        if(!physics || !collider)
            return;

        if (player->isEnemy)
            return;


        //gameState.setDirecInput(player->playerID, NetInputBuffer);


        NetInputBuffer = 0;
        // ----------------------- Attack -----------------------
        if(Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::E)){
            NetInputBuffer |= static_cast<unsigned int>(InputCommand::Attack);
        }

        // ----------------------- Ability -----------------------
        if(Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::LEFT_SHIFT)){
            NetInputBuffer |= static_cast<unsigned int>(InputCommand::Ability);


        }
        // ----------------------- Ultimate -----------------------
        // TODO


        // ----------------------- Movement -----------------------
        if (Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::D)){
            NetInputBuffer |= static_cast<unsigned int>(InputCommand::Right);


        }
        if (Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::A)){
            NetInputBuffer |= static_cast<unsigned int>(InputCommand::Left);


        }
        if(Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::SPACE)){
            NetInputBuffer |= static_cast<unsigned int>(InputCommand::Jump);


        }
        gameState.setPlayerInputHistory(player->playerID, gameState.FrameCount + NET_INPUT_DELAY, NetInputBuffer);
    }
}
