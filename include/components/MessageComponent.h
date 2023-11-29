#ifndef LOTUS_MESSAGECOMPONENT_H
#define LOTUS_MESSAGECOMPONENT_H

#include "Kikan/ecs/components/IComponent.h"
#include "networking/Messages.h"

class MessageComponent : public Kikan::IComponent {
public:
    Message msg{};

    void destroy() override{
        delete this;
    };
};

#endif //LOTUS_MESSAGECOMPONENT_H
