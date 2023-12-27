#ifndef LOTUS_BGSPRITESYSTEM_H
#define LOTUS_BGSPRITESYSTEM_H

#include <Kikan/ecs/systems/ISystem.h>

class BgSpriteSystem : public Kikan::ISystem{
public:
    BgSpriteSystem();

    void update(double dt) override;

private:
};


#endif //LOTUS_BGSPRITESYSTEM_H
