#ifndef LOTUS_TRIGGERSYSTEM_H
#define LOTUS_TRIGGERSYSTEM_H

#include "Kikan/ecs/systems/ISystem.h"

class TriggerSystem : public Kikan::ISystem {
public:
    TriggerSystem();

    void update(double dt) override;

    void addEntity(Kikan::Entity *entity) override;
    void removeEntity(Kikan::Entity *entity) override;

private:

    std::vector<Kikan::Entity*> _t_entities;
    std::vector<Kikan::Entity*> _s_entities;
    std::vector<Kikan::Entity*> _d_entities;
};


#endif //LOTUS_TRIGGERSYSTEM_H
