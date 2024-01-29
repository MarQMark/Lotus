#ifndef LOTUS_PLAYERSTATECOMPONENT_H
#define LOTUS_PLAYERSTATECOMPONENT_H

#include <Kikan/ecs/components/IComponent.h>
#include "Constants.h"
#include "SColliderComponent.h"

class PlayerStateComponent : public Kikan::IComponent {
public:

    std::string name = "Player";

    Nation nation = Nation::FIRE;

    int32_t playerID = -1;

    uint8_t facing = 0; // 0 = right, 1 = left
    bool onGround = false;
    bool isMoving = false;
    bool canMove = true;
    bool canInput = true;
    bool isEnemy = false;

    // Not actually falling but basically pressing S
    bool isFalling = false;

    float movMulti = 1.f;
    float jumpMulti = 1.f;

    float ultCharge = 0;

    // A component should not have another component but since is nothing like unique ids I can't come up with something better. Too bad!
    SColliderComponent* excludeCollider = nullptr;

    void destroy() override{
        delete this;
    }
};

#endif //LOTUS_PLAYERSTATECOMPONENT_H
