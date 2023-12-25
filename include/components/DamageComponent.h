#ifndef LOTUS_DAMAGECOMPONENT_H
#define LOTUS_DAMAGECOMPONENT_H

#include <Kikan/ecs/components/IComponent.h>

class DamageComponent : public Kikan::IComponent{
public:
    double damage = 0;

    uint32_t effectID = 0;
    double effectDuration = 0;

    void destroy() override{
        delete this;
    }
};

#endif //LOTUS_DAMAGECOMPONENT_H
