#ifndef LOTUS_COLLISIONSYSTEM_H
#define LOTUS_COLLISIONSYSTEM_H

#include "Kikan/ecs/systems/ISystem.h"

class CollisionSystem : public Kikan::ISystem{
public:
    CollisionSystem();

    void addEntity(Kikan::Entity *entity) override;

    void update(double dt) override;
private:
    enum Direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        NONE,
    };

    void updateColliderPosition();
    void updateTransformPosition();

    Direction collidesDirectionAABB(Kikan::Entity* dEntity, Kikan::Entity* sEntity);
    void handleCleanCollision(Direction direction, Kikan::Entity* dEntity, Kikan::Entity* sEntity);

    std::vector<Kikan::Entity*> _d_entities; // entities with dynamic collider
    std::vector<Kikan::Entity*> _s_entities; // entities with static collider
};


#endif //LOTUS_COLLISIONSYSTEM_H
