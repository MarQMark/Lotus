#ifndef LOTUS_NETWORKINGCLIENTSYSTEM_H
#define LOTUS_NETWORKINGCLIENTSYSTEM_H

#include "Kikan/ecs/systems/ISystem.h"

class NetworkingClientSystem : public Kikan::ISystem {
public:
    NetworkingClientSystem();

    void update(double dt) override;
private:
    bool _enabled = false;
    int _sockfd = -1;

};


#endif //LOTUS_NETWORKINGCLIENTSYSTEM_H
