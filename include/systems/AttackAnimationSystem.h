#ifndef LOTUS_ATTACKANIMATIONSYSTEM_H
#define LOTUS_ATTACKANIMATIONSYSTEM_H

#include <Kikan/ecs/systems/ISystem.h>

class AttackAnimationSystem : public Kikan::ISystem {
public:
    AttackAnimationSystem();

    void update(double dt) override;
private:
};


#endif //LOTUS_ATTACKANIMATIONSYSTEM_H
