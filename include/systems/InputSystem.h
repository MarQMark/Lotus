#ifndef LOTUS_INPUTSYSTEM_H
#define LOTUS_INPUTSYSTEM_H
#include <Kikan/ecs/systems/ISystem.h>

class InputSystem : public Kikan::ISystem {
public:
    InputSystem();

    void update(double dt) override;
private:
};

#endif //LOTUS_INPUTSYSTEM_H
