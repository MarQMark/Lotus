#ifndef LOTUS_CLIENTCOMPONENT_H
#define LOTUS_CLIENTCOMPONENT_H

#include "Kikan/ecs/components/IComponent.h"

class ClientComponent : public Kikan::IComponent {
public:
    int sock_fd = -1;

    void destroy() override{
        delete this;
    };
};


#endif //LOTUS_CLIENTCOMPONENT_H
