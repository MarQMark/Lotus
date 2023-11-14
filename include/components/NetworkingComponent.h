#ifndef LOTUS_NETWORKINGCOMPONENT_H
#define LOTUS_NETWORKINGCOMPONENT_H

#include "Kikan/ecs/components/IComponent.h"

class NetworkingComponent : public Kikan::IComponent {

    int sock_fd = -1;

    void destroy() override{
        delete this;
    };
};

#endif //LOTUS_NETWORKINGCOMPONENT_H
