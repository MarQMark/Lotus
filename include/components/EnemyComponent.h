#ifndef LOTUS_ENEMYCOMPONENT_H
#define LOTUS_ENEMYCOMPONENT_H

#include "Kikan/ecs/components/IComponent.h"

class EnemyComponent : public Kikan::IComponent {
public:
    int32_t enemyID = -1;

    void destroy() override{
        delete this;
    };
};

#endif //LOTUS_ENEMYCOMPONENT_H
