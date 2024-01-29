#ifndef LOTUS_GAMELOOPSYSTEM_H
#define LOTUS_GAMELOOPSYSTEM_H

#include "Kikan/ecs/systems/ISystem.h"

class GameLoopSystem : public Kikan::ISystem{
public:
    GameLoopSystem();

    void update(double dt) override;

    static double Countdown;
private:
};


#endif //LOTUS_GAMELOOPSYSTEM_H
