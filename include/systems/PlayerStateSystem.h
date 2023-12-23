#ifndef LOTUS_PLAYERSTATESYSTEM_H
#define LOTUS_PLAYERSTATESYSTEM_H

#include "Kikan/ecs/systems/ISystem.h"

class PlayerStateSystem : public Kikan::ISystem {
public:
    PlayerStateSystem();

    void update(double dt) override;
private:
};


#endif //LOTUS_PLAYERSTATESYSTEM_H
