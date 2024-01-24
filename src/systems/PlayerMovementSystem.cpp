#include "systems/PlayerMovementSystem.h"
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

PlayerMovementSystem::PlayerMovementSystem() {
    includeSingle(PlayerComponent);
}

void PlayerMovementSystem::update(double dt) {
    GameState& gameState = GameState::getInstance();
    if(!gameState.UpdateGame)
        return;

    for (Kikan::Entity *e: _entities) {
        auto *transform = e->getComponent<Kikan::Transform>();
        auto *physics = e->getComponent<Kikan::Physics>();
        auto *collider = e->getComponent<DColliderComponent>();
        auto *player = e->getComponent<PlayerStateComponent>();

        if (transform->position.y < -100) {
            transform->position.y = 150;
            transform->position.x = 500;
            physics->velocity = glm::vec2(0);
            physics->acceleration = glm::vec2(0);
        }

        if (!physics || !collider)
            return;

        if (!player->canInput)
            return;

        // ----------------------- Attack -----------------------
        if (gameState.getPlayerInput(player->playerID) & static_cast<unsigned int>(InputCommand::Attack)) {
            auto *effect = e->getComponent<EffectComponent>();
            if (effect && !effect->effects.count(EffectComponent::ID::ATTACK_COOLDOWN) &&
                !effect->effects.count(EffectComponent::ID::ATTACK_CAST)) {
                effect->effects[EffectComponent::ID::ATTACK_CAST] = ATTACK_CAST[player->nation];
            }
        }

        // ----------------------- Ability -----------------------
        if (gameState.getPlayerInput(player->playerID) & static_cast<unsigned int>(InputCommand::Ability)) {
            auto *effect = e->getComponent<EffectComponent>();
            if (effect && !effect->effects.count(EffectComponent::ID::ABILITY_COOLDOWN) &&
                !effect->effects.count(EffectComponent::ID::ABILITY_CAST)) {
                effect->effects[EffectComponent::ID::ABILITY_CAST] = ABILITY_CAST[player->nation];
            }
        }


#ifdef ENABLE_ULT
        // ----------------------- Ultimate -----------------------
        if(Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::Q)){
            auto* effect = e->getComponent<EffectComponent>();
            if(player->ultCharge == 100 && effect && !effect->effects.count(EffectComponent::ID::ULT_COOLDOWN) && !effect->effects.count(EffectComponent::ID::ULT_CAST)){
                effect->effects[EffectComponent::ID::ULT_CAST] = ULT_CAST[player->nation];
                player->ultCharge = 0;
            }
        }
#endif

#ifdef DEBUG
        // ----------------------- Debug -----------------------
        if (Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::N)) {
            auto *effect = e->getComponent<EffectComponent>();
            if (effect && !effect->effects.count(42)) {
#ifdef ENABLE_WATER
                player->nation = Nation((player->nation + 1) % 4);
#else
                player->nation = Nation((player->nation + 1) % 3);
#endif
                effect->effects[42] = 500;
            }
        }
#endif

        // ----------------------- Movement -----------------------
        if (!player->canMove)
            return;


        if (gameState.getPlayerInput(player->playerID) & static_cast<unsigned int>(InputCommand::Right)) {
            physics->acceleration.x += MOVEMENT_SPEED * player->movMulti;
            player->facing = 0; // faces right
        }
        if (gameState.getPlayerInput(player->playerID) & static_cast<unsigned int>(InputCommand::Left)) {
            physics->acceleration.x += -MOVEMENT_SPEED * player->movMulti;
            player->facing = 1; // faces left
        }
        if (gameState.getPlayerInput(player->playerID) & static_cast<unsigned int>(InputCommand::Jump)) {
            if (collider->hasCollidedB)
                physics->velocity.y = JUMP_FORCE * player->jumpMulti;
        }
        if (gameState.getPlayerInput(player->playerID) & static_cast<unsigned int>(InputCommand::Super)) {
            auto *effect = e->getComponent<EffectComponent>();
            if (effect && !effect->effects.count(EffectComponent::ID::FALL_COOLDOWN)) {
                effect->effects[EffectComponent::ID::FALL_COOLDOWN] = 200;
                player->isFalling = true;
            } else {
                player->isFalling = false;
            }
        } else {
            player->isFalling = false;
        }
    }

    gameState.UpdateGame = false;

}
