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

        if (Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::D)){
            physics->acceleration.x += MOVEMENT_SPEED;
            player->facing = 0; // faces right
        }
        if (Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::A)){
            physics->acceleration.x += -MOVEMENT_SPEED;
            player->facing = 1; // faces left
        }

        if(Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::SPACE)){
            if(collider->hasCollidedB)
                physics->velocity.y = JUMP_FORCE;
        }

        if(Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::E)){
            auto* effect = e->getComponent<EffectComponent>();
            if(effect && !effect->effects.count(EffectComponent::ID::FIRE_ATTACK_COOLDOWN)){
                auto* attack = Spawner::spawnAttack();
                attack->getComponent<Kikan::Transform>()->position.y = transform->position.y + collider->offset.y - 20;

                if(player->facing == 0){
                    attack->getComponent<Kikan::Transform>()->position.x = transform->position.x + collider->offset.x + collider->dimensions.x + 10;
                }
                else {
                    attack->getComponent<Kikan::Transform>()->position.x = transform->position.x + collider->offset.x - attack->getComponent<TriggerComponent>()->dimensions.x - 10;
                    attack->getComponent<Kikan::Physics>()->velocity.x *= -1;
                    attack->getComponent<TriggerComponent>()->impulse.x *= -1;
                }

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

                effect->effects[EffectComponent::ID::FIRE_ATTACK_COOLDOWN] = FIRE_ATTACK_COOL;
                Kikan::Engine::Kikan()->getECS()->getScene()->addEntity(attack);
            }
        }

        if(Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::N)){
            auto* effect = e->getComponent<EffectComponent>();
            if(effect && !effect->effects.count(42)) {
                player->nation = Nation((player->nation + 1) % 4);
                effect->effects[42] = 500;
            }
        }
    }
}
