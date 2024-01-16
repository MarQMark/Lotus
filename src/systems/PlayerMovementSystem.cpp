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

PlayerMovementSystem::PlayerMovementSystem() {
    includeSingle(PlayerComponent);
}

void PlayerMovementSystem::update(double dt) {
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
        if(Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::E)){
            auto* effect = e->getComponent<EffectComponent>();
            if(effect && !effect->effects.count(EffectComponent::ID::ATTACK_COOLDOWN) && !effect->effects.count(EffectComponent::ID::ATTACK_CAST)){

                // send attack to server
                //{
                //    auto* entity = new Kikan::Entity;
                //    auto* msgComponent = new MessageComponent();
                //    msgComponent->msg.hdr.id = MessageID::Attack;
                //    msgComponent->msg.hdr.len = sizeof(BAttack);
                //    msgComponent->msg.body.attack.nation = 0;
                //    msgComponent->msg.body.attack.direction = player->facing;
                //    msgComponent->msg.body.attack.x = attack->getComponent<Kikan::Transform>()->position.x;
                //    msgComponent->msg.body.attack.y = attack->getComponent<Kikan::Transform>()->position.y;
                //    entity->addComponent(msgComponent);
                //    Kikan::Engine::Kikan()->getECS()->getScene()->addEntity(entity);
                //}

                effect->effects[EffectComponent::ID::ATTACK_CAST] = ATTACK_CAST[player->nation];
            }
        }

        // ----------------------- Ability -----------------------
        if(Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::LEFT_SHIFT)){
            auto* effect = e->getComponent<EffectComponent>();
            if(effect && !effect->effects.count(EffectComponent::ID::ABILITY_COOLDOWN) && !effect->effects.count(EffectComponent::ID::ABILITY_CAST)){
                effect->effects[EffectComponent::ID::ABILITY_CAST] = ABILITY_CAST[player->nation];
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
        if(!player->canMove)
            return;

        if (Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::D)){
            physics->acceleration.x += MOVEMENT_SPEED * player->movMulti;
            player->facing = 0; // faces right
        }
        if (Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::A)){
            physics->acceleration.x += -MOVEMENT_SPEED * player->movMulti;
            player->facing = 1; // faces left
        }
        if(Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::SPACE)){
            if(collider->hasCollidedB)
                physics->velocity.y = JUMP_FORCE * player->jumpMulti;
        }
    }
}
