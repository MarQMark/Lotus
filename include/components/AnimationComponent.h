#ifndef LOTUS_ANIMATIONCOMPONENT_H
#define LOTUS_ANIMATIONCOMPONENT_H

#include <Kikan/ecs/components/IComponent.h>
#include <chrono>
#include "util/AnimationManager.h"

class AnimationComponent : public Kikan::IComponent {
public:
    Animation* animation;

    double currentFrame = 0;
    std::chrono::high_resolution_clock::time_point lastTime;

    void destroy() override{
        delete this;
    };
};

#endif //LOTUS_ANIMATIONCOMPONENT_H
