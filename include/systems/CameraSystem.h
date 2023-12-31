#ifndef LOTUS_CAMERASYSTEM_H
#define LOTUS_CAMERASYSTEM_H

#include "Kikan/ecs/systems/ISystem.h"

#include "Kikan/renderer/stdRenderer/Camera.h"


class CameraSystem : public Kikan::ISystem{
public:
    CameraSystem();

    void update(double dt) override;

private:
    Kikan::Camera _camera;
};


#endif //LOTUS_CAMERASYSTEM_H
