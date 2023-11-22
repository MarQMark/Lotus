#ifndef LOTUS_DYNAMICCOLLIDERCOMPONENT_H
#define LOTUS_DYNAMICCOLLIDERCOMPONENT_H

#include "Kikan/ecs/components/IComponent.h"

class DColliderComponent : public Kikan::IComponent {
public:
    glm::vec2 position;
    glm::vec2 lastPosition;

    glm::vec2 dimensions;

    // TODO: exchange with single bits
    bool hasCollidedT = false;
    bool hasCollidedB = false;
    bool hasCollidedL = false;
    bool hasCollidedR = false;

    void destroy() override{
        delete this;
    };
};

#endif //LOTUS_DYNAMICCOLLIDERCOMPONENT_H
