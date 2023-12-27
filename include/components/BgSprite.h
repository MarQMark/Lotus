#ifndef LOTUS_BGSPRITE_H
#define LOTUS_BGSPRITE_H

#include <Kikan/ecs/components/IComponent.h>

class BgSprite : public Kikan::IComponent {
public:
    enum ID{
        CLOUDS,
        TRAIN
    };

    glm::vec2 vel = glm::vec2(0);
    ID id;

    void destroy() override{
        delete this;
    }
};

#endif //LOTUS_BGSPRITE_H
