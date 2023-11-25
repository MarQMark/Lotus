#ifndef LOTUS_SCOLLIDERCOMPONENT_H
#define LOTUS_SCOLLIDERCOMPONENT_H

#include "Kikan/ecs/components/IComponent.h"

class SColliderComponent : public Kikan::IComponent {
public:
    // Does not have to be set
    glm::vec2 position;

    glm::vec2 offset = glm::vec2(0);;
    glm::vec2 dimensions;

    void destroy() override{
        delete this;
    };
};

#endif //LOTUS_SCOLLIDERCOMPONENT_H
