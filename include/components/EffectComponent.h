#ifndef LOTUS_EFFECTCOMPONENT_H
#define LOTUS_EFFECTCOMPONENT_H

#include "Kikan/ecs/components/IComponent.h"
#include <map>

class EffectComponent : public Kikan::IComponent {
public:
    enum ID{
        ATTACK_CAST,
        ATTACK_COOLDOWN,
        ABILITY_CAST,
        ABILITY_COOLDOWN,
        ULT_CAST,
        ULT_COOLDOWN,

        BLOCK_MOV,
        BLOCK_INPUT,

        FIRE_ABILITY,
    };

    std::map<uint32_t, double> effects;

    void destroy() override{
        delete this;
    };
};

#endif //LOTUS_EFFECTCOMPONENT_H
