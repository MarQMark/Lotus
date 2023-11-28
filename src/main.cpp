#include "Kikan/Engine.h"
#include "Kikan/renderer/stdRenderer/Camera.h"
#include "Kikan/ecs/systems/SpriteRenderSystem.h"
#include "Kikan/ecs/components/LineQuadSprite.h"
#include "components/PlayerComponent.h"
#include "systems/PlayerMovementSystem.h"
#include "systems/NetworkingServerSystem.h"
#include "systems/NetworkingClientSystem.h"
#include "systems/PhysicsSystem.h"
#include "Kikan/ecs/components/Physics.h"
#include "components/SColliderComponent.h"
#include "components/DColliderComponent.h"
#include "systems/CollisionSystem.h"
#include "Constants.h"
#include "util/Spawner.h"
#include "systems/CameraSystem.h"

int WinMain() {
    Kikan::Engine::init();
    Kikan::Engine* engine = Kikan::Engine::Kikan();


    auto* spriteSystem = new Kikan::SpriteRenderSystem();

    engine->getECS()->getScene()->addSystem(spriteSystem);

    auto* cameraSystem = new CameraSystem();
    auto* physicsSystem = new PhysicsSystem();
    physicsSystem->gravity = GRAVITY;
    auto* movSystem = new PlayerMovementSystem();
    auto* clientSystem = new NetworkingClientSystem();
    auto* collisionSystem = new CollisionSystem();

    engine->getECS()->getScene()->addSystem(cameraSystem);
    engine->getECS()->getScene()->addSystem(physicsSystem);
    engine->getECS()->getScene()->addSystem(movSystem);
    engine->getECS()->getScene()->addSystem(clientSystem);
    engine->getECS()->getScene()->addSystem(collisionSystem);

    auto* serverSystem = new NetworkingServerSystem();
    engine->getECS()->createThread(10, 100);
    engine->getECS()->addThreadedSystem(serverSystem, 0);

    // Player
    {
        auto* entity = Spawner::spawnPlayer();
        entity->getComponent<Kikan::Transform>()->position = glm::vec3(500, 300, 0);
        engine->getECS()->getScene()->addEntity(entity);
    }

    // Ground
    {
        auto* entity = new Kikan::Entity();
        entity->getComponent<Kikan::Transform>()->position = glm::vec3(50, 20, 0);

        auto* sprite = new Kikan::LineQuadSprite();
        sprite->dimensions = glm::vec2(900, 20);
        sprite->color = glm::vec4(.4, .4, .4, 1);
        sprite->thickness = 5;
        entity->addComponent(sprite);

        auto* collider = new SColliderComponent();
        collider->dimensions = glm::vec2(900, 20);
        entity->addComponent(collider);

        engine->getECS()->getScene()->addEntity(entity);
    }

    std::string title = "Lotus\n";
    engine->setTitle(title);

    while (engine->shouldRun()) {
        engine->update();
    }

    return 0;
}


#ifdef __linux__
int main(){
    WinMain();
}
#endif