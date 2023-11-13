#include "systems/PlayerMovementSystem.h"
#include "components/PlayerComponent.h"
#include "Kikan/ecs/Entity.h"
#include "Kikan/ecs/components/LineQuadSprite.h"
#include "Kikan/Engine.h"
#include "Kikan/input/Input.h"

PlayerMovementSystem::PlayerMovementSystem() {
    singleInclude(PlayerComponent);
}

void PlayerMovementSystem::update(double dt) {
    for (Kikan::Entity *e: _entities) {
        auto *spite = e->getComponent<Kikan::LineQuadSprite>();
        if(spite){
            if (Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::W)){
                spite->position.y+=10;
            }
            if (Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::S)){
                spite->position.y-=10;
            }

            if (Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::D)){
                spite->position.x+=10;
            }
            if (Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::A)){
                spite->position.x-=10;
            }
        }
    }
}
