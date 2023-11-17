#include "Kikan/Engine.h"
#include "Kikan/renderer/stdRenderer/Camera.h"
#include "Kikan/ecs/systems/SpriteRenderSystem.h"
#include "Kikan/ecs/components/LineQuadSprite.h"
#include "components/PlayerComponent.h"
#include "systems/PlayerMovementSystem.h"
#include "systems/NetworkingServerSystem.h"
#include "systems/NetworkingClientSystem.h"
#include "Kikan/ecs/systems/PhysicsSystem.h"
#include "Kikan/ecs/components/Physics.h"

int WinMain() {
    Kikan::Engine::init();
    Kikan::Engine* engine = Kikan::Engine::Kikan();

    Kikan::Renderer::Camera camera;
    camera.scale(2/1280., 2/720.);
    camera.translate(-1280/2., -720/2.);
    ((Kikan::Renderer::StdRenderer*)engine->getRenderer())->mvp = camera.matrix();

    auto* spriteSystem = new Kikan::SpriteRenderSystem();
    //auto* physicsSystem = new Kikan::PhysicsSystem();

    auto* movSystem = new PlayerMovementSystem();
    auto* serverSystem = new NetworkingServerSystem();
    auto* clientSystem = new NetworkingClientSystem();

    engine->getECS()->getScene()->addSystem(spriteSystem);
    //engine->getECS()->getScene()->addSystem(physicsSystem);

    engine->getECS()->getScene()->addSystem(movSystem);
    engine->getECS()->getScene()->addSystem(serverSystem);
    engine->getECS()->getScene()->addSystem(clientSystem);


    auto* entity = new Kikan::Entity();
    entity->getComponent<Kikan::Transform>()->position = glm::vec3(50, 20, 0);
    auto* physics = new Kikan::Physics();
    auto* sprite = new Kikan::LineQuadSprite();
    sprite->dimensions = glm::vec2(100, 100);
    sprite->color = glm::vec4(.4, .5, .8, 1);
    sprite->thickness = 6;
    auto* player = new PlayerComponent();
    entity->addComponent(player);
    entity->addComponent(sprite);
    entity->addComponent(physics);
    engine->getECS()->getScene()->addEntity(entity);

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