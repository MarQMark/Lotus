#include "systems/EffectSystem.h"

#include "components/EffectComponent.h"
#include "util/Spawner.h"
#include "Kikan/Engine.h"
#include "components/PlayerStateComponent.h"
#include "components/HealthComponent.h"

EffectSystem::EffectSystem() {
    includeSingle(EffectComponent);
}

void endPlayerEffect(Kikan::Entity* e, EffectComponent::ID id){
    auto* transform = e->getComponent<Kikan::Transform>();
    auto* player = e->getComponent<PlayerStateComponent>();
    auto* effect = e->getComponent<EffectComponent>();

    switch (id) {
        case EffectComponent::ID::ATTACK_CAST: {
            auto* attack = Spawner::spawnAttack(transform->position, player->nation, player->facing);
            effect->effects[EffectComponent::ID::ATTACK_COOLDOWN] = ATTACK_COOLDOWN[player->nation];;
            Kikan::Engine::Kikan()->getECS()->getScene()->addEntity(attack);
            break;
        }
        case EffectComponent::ID::ABILITY_CAST: {
            switch (player->nation) {
                case Nation::FIRE:
                    effect->effects[EffectComponent::ID::FIRE_ABILITY] = 5000;
                    break;
                case Nation::EARTH:{
                    auto* ability = Spawner::spawnEarthAbility(transform->position, player->facing);
                    Kikan::Engine::Kikan()->getECS()->getScene()->addEntity(ability);
                }
                    break;
                case Nation::AIR:
                {
                    auto* attack = Spawner::spawnAttack(transform->position, player->nation, 0);
                    Kikan::Engine::Kikan()->getECS()->getScene()->addEntity(attack);
                    attack = Spawner::spawnAttack(transform->position, player->nation, 1);
                    Kikan::Engine::Kikan()->getECS()->getScene()->addEntity(attack);
                }
                    break;
                case Nation::WATER:{
                    auto* health = e->getComponent<HealthComponent>();
                    health->health = std::min(health->health + HEAL_HEALTH, health->maxHealth);
                    effect->effects[EffectComponent::ID::ABILITY_COOLDOWN] = ABILITY_COOLDOWN[player->nation];
                    break;
                }
            }
        }
        case EffectComponent::ID::FIRE_ABILITY:
            player->movMulti = 1.f;
            player->jumpMulti = 1.f;
            break;
        default:
            break;
    }
}

void EffectSystem::update(double dt) {
    for (auto* entity : _entities) {
        auto* effectComponent = entity->getComponent<EffectComponent>();
        for(auto effect = effectComponent->effects.begin(); effect != effectComponent->effects.cend(); ){

            if(effect->second <= 0){
                if(effect->first == EffectComponent::ID::SELF_DESTRUCT)
                    _to_be_destroyed.push_back(entity);

                if(entity->getComponent<PlayerStateComponent>())
                    endPlayerEffect(entity, (EffectComponent::ID)effect->first);
                effectComponent->effects.erase(effect++);
            }
            else{
                effect->second -= dt;
                ++effect;
            }
        }
    }

    for(uint32_t i = 0; i < _to_be_destroyed.size(); i++){
        // TODO: FIX memory leak
        Kikan::Engine::Kikan()->getECS()->getScene()->removeEntity(_to_be_destroyed[i]);
    }
    _to_be_destroyed.clear();
}
