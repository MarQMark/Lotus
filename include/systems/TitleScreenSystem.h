#ifndef LOTUS_TITLESCREENSYSTEM_H
#define LOTUS_TITLESCREENSYSTEM_H

#include "Kikan/ecs/systems/ISystem.h"

class TitleScreenSystem : public Kikan::ISystem {
public:
    TitleScreenSystem();

    void update(double dt) override;

private:
    const double _max_duration = 3000;
    const double _fade_in = 500;
    const double _fade_out = 1000;

    double _duration = _max_duration;
};


#endif //LOTUS_TITLESCREENSYSTEM_H
