#ifndef LOTUS_HEALTHBARSYSTEM_H
#define LOTUS_HEALTHBARSYSTEM_H

#include <Kikan/ecs/systems/ISystem.h>

class HealthbarSystem : public Kikan::ISystem {
public:
    HealthbarSystem();

    void update(double dt) override;
private:
};


#endif //LOTUS_HEALTHBARSYSTEM_H
