#ifndef LOTUS_ENEMYSYSTEM_H
#define LOTUS_ENEMYSYSTEM_H

#include "Kikan/ecs/systems/ISystem.h"

class EnemySystem : public Kikan::ISystem {
    EnemySystem();

    void update(double dt) override;
};


#endif //LOTUS_ENEMYSYSTEM_H
