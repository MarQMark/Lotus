#ifndef LOTUS_EFFECTSYSTEM_H
#define LOTUS_EFFECTSYSTEM_H

#include "Kikan/ecs/systems/ISystem.h"

class EffectSystem : public Kikan::ISystem{

public:
    EffectSystem();

    void update(double dt) override;

private:

};


#endif //LOTUS_EFFECTSYSTEM_H
