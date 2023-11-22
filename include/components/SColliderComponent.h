#ifndef LOTUS_SCOLLIDERCOMPONENT_H
#define LOTUS_SCOLLIDERCOMPONENT_H

#include "Kikan/ecs/components/IComponent.h"

class SColliderComponent : public Kikan::IComponent {
public:
    glm::vec2 dimensions;

    void destroy() override{
        delete this;
    };
};

#endif //LOTUS_SCOLLIDERCOMPONENT_H
