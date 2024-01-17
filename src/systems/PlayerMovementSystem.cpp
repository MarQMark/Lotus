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

        if(!physics || !collider)
            return;

        if(!player->canInput)
            return;

        // ----------------------- Attack -----------------------
        if(gameState.getPlayerInput(player->playerID) & static_cast<unsigned int>(InputCommand::Attack)){
            auto* effect = e->getComponent<EffectComponent>();
            if(effect && !effect->effects.count(EffectComponent::ID::ATTACK_COOLDOWN)){
                auto* attack = Spawner::spawnAttack(transform->position, player->nation, player->facing);

                // send attack to server
                {
                    auto* entity = new Kikan::Entity;
                    auto* msgComponent = new MessageComponent();
                    msgComponent->msg.hdr.id = MessageID::Attack;
                    msgComponent->msg.hdr.len = sizeof(BAttack);
                    msgComponent->msg.body.attack.nation = 0;
                    msgComponent->msg.body.attack.direction = player->facing;
                    msgComponent->msg.body.attack.x = attack->getComponent<Kikan::Transform>()->position.x;
                    msgComponent->msg.body.attack.y = attack->getComponent<Kikan::Transform>()->position.y;
                    entity->addComponent(msgComponent);
                    Kikan::Engine::Kikan()->getECS()->getScene()->addEntity(entity);
                }

                effect->effects[EffectComponent::ID::ATTACK_CAST] = FIRE_ATTACK_CAST;
                effect->effects[EffectComponent::ID::ATTACK_COOLDOWN] = FIRE_ATTACK_COOL;
                Kikan::Engine::Kikan()->getECS()->getScene()->addEntity(attack);
            }
        }

        // ----------------------- Ability -----------------------
        if(gameState.getPlayerInput(player->playerID) & static_cast<unsigned int>(InputCommand::Ability)){
            auto* effect = e->getComponent<EffectComponent>();
            if(effect && !effect->effects.count(EffectComponent::ID::ABILITY_COOLDOWN)){
                switch (player->nation) {
                    case Nation::FIRE:
                        effect->effects[EffectComponent::ID::FIRE_ABILITY] = 5000;
                        break;
                    case Nation::EARTH:
                        break;
                    case Nation::AIR:
                    {
                        auto* attack = Spawner::spawnAttack(transform->position, player->nation, 0);
                        Kikan::Engine::Kikan()->getECS()->getScene()->addEntity(attack);
                        attack = Spawner::spawnAttack(transform->position, player->nation, 1);
                        Kikan::Engine::Kikan()->getECS()->getScene()->addEntity(attack);
                    }
                        break;
                    case Nation::WATER:
                        effect->effects[EffectComponent::ID::BLOCK_INPUT] = 2000;
                        break;
                }

                effect->effects[EffectComponent::ID::ABILITY_CAST] = FIRE_ATTACK_CAST;
                effect->effects[EffectComponent::ID::ABILITY_COOLDOWN] = FIRE_ATTACK_COOL;
            }
        }


        // ----------------------- Ultimate -----------------------
        // TODO

        // ----------------------- Debug -----------------------
        if(Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::N)){
            auto* effect = e->getComponent<EffectComponent>();
            if(effect && !effect->effects.count(42)) {
                player->nation = Nation((player->nation + 1) % 4);
                effect->effects[42] = 500;
            }
        }

        // ----------------------- Movement -----------------------
        //kikanPrint(reinterpret_cast<const char *>(gameState.getPlayerInput(player->playerID)));
        //kikanPrint("\n");
        //kikanPrint("playerId:");
        //kikanPrint(reinterpret_cast<const char *>(player->playerID));
        //kikanPrint("\n");
       // std::cout << gameState.getPlayerInput(player->playerID) << std::endl;

        if(!player->canMove)
            return;
        if(!player->isEnemy)
        {
            std::cout << gameState.getPlayerInput(player->playerID) << std::endl;

        }
        if (gameState.getPlayerInput(player->playerID) & static_cast<unsigned int>(InputCommand::Right)){
            physics->acceleration.x += MOVEMENT_SPEED * player->movMulti;
            player->facing = 0; // faces right
        }
        if (gameState.getPlayerInput(player->playerID) & static_cast<unsigned int>(InputCommand::Left)){
            physics->acceleration.x += -MOVEMENT_SPEED * player->movMulti;
            player->facing = 1; // faces left
        }
        if(gameState.getPlayerInput(player->playerID) & static_cast<unsigned int>(InputCommand::Jump)){
            if(collider->hasCollidedB)
                physics->velocity.y = JUMP_FORCE * player->jumpMulti;
        }
    }
    gameState.UpdateGame = false;
}
