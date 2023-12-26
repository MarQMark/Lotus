#ifndef LOTUS_HEALTHCOMPONENT_H
#define LOTUS_HEALTHCOMPONENT_H

#include <Kikan/ecs/components/IComponent.h>
#include "Constants.h"

class HealthComponent : public Kikan::IComponent {
public:
    double maxHealth = PLAYER_HEALTH;
    double health = maxHealth;

    void destroy() override{
        delete this;
    }
};

#endif //LOTUS_HEALTHCOMPONENT_H
