#include "systems/PlayerMovementSystem.h"
#include "components/PlayerComponent.h"
#include "Kikan/ecs/Entity.h"
#include "Kikan/Engine.h"
#include "Kikan/input/Input.h"
#include "Kikan/ecs/components/Physics.h"
#include "components/DColliderComponent.h"
#include "Constants.h"

PlayerMovementSystem::PlayerMovementSystem() {
    singleInclude(PlayerComponent);
}

void PlayerMovementSystem::update(double dt) {
    for (Kikan::Entity *e: _entities) {
        auto *transform = e->getComponent<Kikan::Transform>();
        auto *physics = e->getComponent<Kikan::Physics>();
        auto *collider = e->getComponent<DColliderComponent>();
        if(!physics || !collider)
            return;

        if (Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::D)){
            physics->acceleration.x += MOVEMENT_SPEED;
        }
        if (Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::A)){
            physics->acceleration.x += -MOVEMENT_SPEED;
        }

        if(Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::SPACE)){
            if(collider->hasCollidedB)
                physics->velocity.y = JUMP_FORCE;
        }
    }
}
