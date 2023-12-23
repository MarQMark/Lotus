#include "systems/PlayerStateSystem.h"
#include "components/PlayerStateComponent.h"
#include "components/DColliderComponent.h"
#include "Kikan/ecs/components/Physics.h"

PlayerStateSystem::PlayerStateSystem() {
    includeSingle(PlayerStateComponent);
}

void PlayerStateSystem::update(double dt) {
    for (auto* e : _entities) {
        auto* player = e->getComponent<PlayerStateComponent>();

        auto* collider = e->getComponent<DColliderComponent>();
        if(collider)
            player->onGround = collider->hasCollidedB;

        auto* physics = e->getComponent<Kikan::Physics>();
        if(physics)
            player->isMoving = (std::abs(physics->velocity.x) > 0.2f);

    }
}
