#ifndef LOTUS_PLAYERMOVEMENTSYSTEM_H
#define LOTUS_PLAYERMOVEMENTSYSTEM_H

#include "Kikan/ecs/systems/ISystem.h"

class PlayerMovementSystem : public Kikan::ISystem{
public:
    PlayerMovementSystem();

    void update(double dt) override;
private:
};


#endif //LOTUS_PLAYERMOVEMENTSYSTEM_H
