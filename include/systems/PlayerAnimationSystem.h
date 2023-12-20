#ifndef LOTUS_PLAYERANIMATIONSYSTEM_H
#define LOTUS_PLAYERANIMATIONSYSTEM_H

#include "Kikan/ecs/systems/ISystem.h"
#include "util/AnimationManager.h"

class PlayerAnimationSystem : public Kikan::ISystem {
public:
    PlayerAnimationSystem();

    void update(double dt) override;

private:
    Animation* _last_animation;
};


#endif //LOTUS_PLAYERANIMATIONSYSTEM_H
