#ifndef LOTUS_NETWORKINGSERVERSYSTEM_H
#define LOTUS_NETWORKINGSERVERSYSTEM_H

#include "Kikan/ecs/systems/ISystem.h"

class NetworkingServerSystem : public Kikan::ISystem {
public:
    NetworkingServerSystem();

    void update(double dt) override;
private:
    bool _enabled = false;

    int _passive_sock;
};

#endif //LOTUS_NETWORKINGSERVERSYSTEM_H
