#include "scenes/Scenes.h"
#include "Kikan/ecs/Scene.h"
#include "systems/CameraSystem.h"
#include "Kikan/ecs/systems/SpriteRenderSystem.h"
#include "systems/TitleScreenSystem.h"
#include "Kikan/ecs/components/AASprite.h"
#include "util/ResourceManager.h"
#include "Kikan/ecs/components/QuadSprite.h"

void addTitleScene(){
    auto* scene = new Kikan::Scene(SCENE_TITLE);

    auto* cameraSystem = new CameraSystem();
    scene->addSystem(cameraSystem);

    auto* spriteSystem = new Kikan::SpriteRenderSystem();
    scene->addSystem(spriteSystem);

    auto* titleScreenSystem = new TitleScreenSystem();
    scene->addSystem(titleScreenSystem);

    {
        auto* entity = new Kikan::Entity;
        entity->getComponent<Kikan::Transform>()->position = glm::vec3(100, 350, 1);
        auto* sprite = new Kikan::AASprite;
        sprite->dimensions = glm::vec2(800.f, 800.f * (123.f / 782.f));
        sprite->textureID = ResourceManager::get<TextureResource>(Resource::ID::TEX_MARK_INDUSTRIES_I)->getID();
        entity->addComponent(sprite);
        scene->addEntity(entity);
    }

    {
        auto* entity = new Kikan::Entity;
        float width = 800.f;
        float height = 400.f;
        auto* sprite = new Kikan::QuadSprite;
        sprite->points[0] = glm::vec2(100, 350);
        sprite->points[1] = glm::vec2(100, 350) + glm::vec2(width,   0);
        sprite->points[2] = glm::vec2(100, 350) + glm::vec2(width,   -height);
        sprite->points[3] = glm::vec2(100, 350) + glm::vec2(0,       -height);
        sprite->color = glm::vec4(0,0,0,1);
        sprite->layer = -.1;
        entity->addComponent(sprite);
        scene->addEntity(entity);
    }

    Kikan::Engine::Kikan()->getECS()->addScene(scene);
}