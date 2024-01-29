#ifndef LOTUS_SCOLLIDERCOMPONENT_H
#define LOTUS_SCOLLIDERCOMPONENT_H

#include "Kikan/ecs/components/IComponent.h"
#include "glm/glm.hpp"

class SColliderComponent : public Kikan::IComponent {
public:
    // Does not have to be set
    glm::vec2 position;

    glm::vec2 offset = glm::vec2(0);
    glm::vec2 dimensions;

    bool playerCanFall = false;

    uint8_t disabledSides = 0;

    void disableSide(uint8_t side){
        disabledSides |= ((uint8_t)1 << side);
    }
    void enableSide(uint8_t side){
        disabledSides &= ~((uint8_t)1 << side);
    }

    void destroy() override{
        delete this;
    };
};

#endif //LOTUS_SCOLLIDERCOMPONENT_H
