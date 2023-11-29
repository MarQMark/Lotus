#ifndef LOTUS_EFFECTCOMPONENT_H
#define LOTUS_EFFECTCOMPONENT_H

#include "Kikan/ecs/components/IComponent.h"
#include <map>

class EffectComponent : public Kikan::IComponent {
public:
    enum ID{
        FIRE_ATTACK_COOLDOWN,

    };

    std::map<uint32_t, double> effects;

    void destroy() override{
        delete this;
    };
};

#endif //LOTUS_EFFECTCOMPONENT_H
