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

int WinMain() {
    Kikan::Engine::init();
    Kikan::Engine* engine = Kikan::Engine::Kikan();

    Kikan::Renderer::Camera camera;
    camera.scale(2/1280., 2/720.);
    camera.translate(-1280/2., -720/2.);
    ((Kikan::Renderer::StdRenderer*)engine->getRenderer())->mvp = camera.matrix();

    auto* spriteSystem = new Kikan::SpriteRenderSystem();

    engine->getECS()->getScene()->addSystem(spriteSystem);

    auto* physicsSystem = new PhysicsSystem();
    physicsSystem->gravity = GRAVITY;
    auto* movSystem = new PlayerMovementSystem();
    auto* clientSystem = new NetworkingClientSystem();
    auto* collisionSystem = new CollisionSystem();

    engine->getECS()->getScene()->addSystem(physicsSystem);
    engine->getECS()->getScene()->addSystem(movSystem);
    engine->getECS()->getScene()->addSystem(clientSystem);
    engine->getECS()->getScene()->addSystem(collisionSystem);

    auto* serverSystem = new NetworkingServerSystem();
    engine->getECS()->createThread(10, 100);
    engine->getECS()->addThreadedSystem(serverSystem, 0);

    // Player
    {
        auto* entity = new Kikan::Entity();
        entity->getComponent<Kikan::Transform>()->position = glm::vec3(500, 200, 0);

        auto* physics = new Kikan::Physics();
        physics->friction.x = PLAYER_FRICTION;
        entity->addComponent(physics);

        auto* sprite = new Kikan::LineQuadSprite();
        sprite->dimensions = glm::vec2(50, 80);
        sprite->color = glm::vec4(.4, .5, .8, 1);
        sprite->thickness = 3;
        entity->addComponent(sprite);

        auto* collider = new DColliderComponent();
        collider->dimensions = glm::vec2(50, 80);
        entity->addComponent(collider);

        auto* player = new PlayerComponent();
        entity->addComponent(player);

        engine->getECS()->getScene()->addEntity(entity);
    }

    // Ground
    {
        auto* entity = new Kikan::Entity();
        entity->getComponent<Kikan::Transform>()->position = glm::vec3(200, 100, 0);

        auto* sprite = new Kikan::LineQuadSprite();
        sprite->dimensions = glm::vec2(880, 50);
        sprite->color = glm::vec4(.4, .4, .4, 1);
        sprite->thickness = 10;
        entity->addComponent(sprite);

        auto* collider = new SColliderComponent();
        collider->dimensions = glm::vec2(880, 50);
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