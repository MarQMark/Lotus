#include "systems/EffectSystem.h"

#include "components/EffectComponent.h"

EffectSystem::EffectSystem() {
    includeSingle(EffectComponent);
}

void EffectSystem::update(double dt) {
    for (auto* entity : _entities) {
        auto* effectComponent = entity->getComponent<EffectComponent>();
        for(auto effect = effectComponent->effects.begin(); effect != effectComponent->effects.cend(); ){

            if(effect->second <= 0){
                effectComponent->effects.erase(effect++);
            }
            else{
                effect->second -= dt;
                ++effect;
            }
        }
    }
}
