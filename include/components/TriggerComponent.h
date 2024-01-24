#ifndef LOTUS_TRIGGERCOMPONENT_H
#define LOTUS_TRIGGERCOMPONENT_H

#include "Kikan/ecs/components/IComponent.h"

class TriggerComponent : public Kikan::IComponent {
public:
    enum TriggerType{
        DUMMY,
        FIRE_A,

        EARTH_ABILITY,
    };

    uint32_t type = TriggerType::DUMMY;

    glm::vec2 offset = glm::vec2(0, 0);

    glm::vec2 dimensions;
    glm::vec2 impulse;

    bool triggerStatic = true;

    void destroy() override{
        delete this;
    };
};

#endif //LOTUS_TRIGGERCOMPONENT_H
