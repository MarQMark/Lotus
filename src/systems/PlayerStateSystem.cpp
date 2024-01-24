#include "systems/PlayerStateSystem.h"
#include "components/PlayerStateComponent.h"
#include "components/DColliderComponent.h"
#include "Kikan/ecs/components/Physics.h"
#include "components/EffectComponent.h"

PlayerStateSystem::PlayerStateSystem() {
    includeSingle(PlayerStateComponent);
}

void PlayerStateSystem::update(double dt) {
    for (auto* e : _entities) {
        auto* player = e->getComponent<PlayerStateComponent>();

        auto* collider = e->getComponent<DColliderComponent>();
        if(collider)
            player->onGround = collider->hasCollidedB;

        auto* physics = e->getComponent<Kikan::Physics>();
        if(physics)
            player->isMoving = (std::abs(physics->velocity.x) > 0.2f);

        auto* effect = e->getComponent<EffectComponent>();
        if(effect){
            player->canMove = !effect->effects.count(EffectComponent::ID::BLOCK_MOV);
            player->canInput = !effect->effects.count(EffectComponent::ID::BLOCK_INPUT);

#ifdef ENABLE_ULT
            if(!effect->effects.count(EffectComponent::ID::ULT_CAST) &&
               !effect->effects.count(EffectComponent::ID::ULT_COOLDOWN))
                player->ultCharge = std::min(player->ultCharge += ULT_GAIN[player->nation] * dt, 100.f);
#endif

            if(effect->effects.count(EffectComponent::ID::FIRE_ABILITY)){
                player->movMulti = 1.5f;
                player->jumpMulti = 1.2f;
            }

            if(effect->effects.count(EffectComponent::ID::ATTACK_CAST)
               || effect->effects.count(EffectComponent::ID::ABILITY_CAST)
#ifdef ENABLE_ULT
               || effect->effects.count(EffectComponent::ID::ULT_CAST)
#endif
               ){
                player->canInput = false;
            }
        }
    }
}
