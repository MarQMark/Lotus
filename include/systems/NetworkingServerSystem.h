#ifndef LOTUS_NETWORKINGSERVERSYSTEM_H
#define LOTUS_NETWORKINGSERVERSYSTEM_H

#include "Kikan/ecs/systems/ISystem.h"

class NetworkingServerSystem : public Kikan::ISystem {
public:
    NetworkingServerSystem();

    void update(double dt) override;
private:

    bool _enabled = false;

    int _passive_sock = -1;

    uint16_t _num_players = 0;
};

#endif //LOTUS_NETWORKINGSERVERSYSTEM_H
