#ifndef LOTUS_PLAYERCOMPONENT_H
#define LOTUS_PLAYERCOMPONENT_H

#include "Kikan/ecs/components/IComponent.h"

class PlayerComponent : public Kikan::IComponent {
public:
    int32_t playerID = -1;

    void destroy() override{
        delete this;
    };
};

#endif //LOTUS_PLAYERCOMPONENT_H
