#ifndef LOTUS_HEALTHBARCOMPONENT_H
#define LOTUS_HEALTHBARCOMPONENT_H

#include <Kikan/ecs/components/IComponent.h>

class HealthbarComponent : public Kikan::IComponent {
public:

    void destroy() override{
        delete this;
    }
};

#endif //LOTUS_HEALTHBARCOMPONENT_H
