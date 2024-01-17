
#ifndef LOTUS_NETWORKSYSTEM_H
#define LOTUS_NETWORKSYSTEM_H

#include "Kikan/ecs/systems/ISystem.h"

class NetworkSystem : public Kikan::ISystem {
public:
    NetworkSystem();

    void update(double dt) override;
};

#endif //LOTUS_NETWORKSYSTEM_H
