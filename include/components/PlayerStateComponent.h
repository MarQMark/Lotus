#ifndef LOTUS_PLAYERSTATECOMPONENT_H
#define LOTUS_PLAYERSTATECOMPONENT_H

#include <Kikan/ecs/components/IComponent.h>
#include "Constants.h"

class PlayerStateComponent : public Kikan::IComponent {
public:

    Nation nation = Nation::FIRE;

    uint8_t facing = 0; // 0 = right, 1 = left
    bool onGround = false;
    bool isMoving = false;
    bool canMove = true;
    bool canInput = true;

    float movMulti = 1.f;
    float jumpMulti = 1.f;

    void destroy() override{
        delete this;
    }
};

#endif //LOTUS_PLAYERSTATECOMPONENT_H
