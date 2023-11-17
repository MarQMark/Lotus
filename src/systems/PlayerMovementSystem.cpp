#include "systems/PlayerMovementSystem.h"
#include "components/PlayerComponent.h"
#include "Kikan/ecs/Entity.h"
#include "Kikan/Engine.h"
#include "Kikan/input/Input.h"

PlayerMovementSystem::PlayerMovementSystem() {
    singleInclude(PlayerComponent);
}

void PlayerMovementSystem::update(double dt) {
    for (Kikan::Entity *e: _entities) {
        auto *transform = e->getComponent<Kikan::Transform>();
        if (Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::W)){
            transform->position.y+=10;
        }
        if (Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::S)){
            transform->position.y-=10;
        }

        if (Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::D)){
            transform->position.x+=10;
        }
        if (Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::A)){
            transform->position.x-=10;
        }
    }
}
