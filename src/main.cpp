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
#include "systems/EffectSystem.h"

#include "Kikan/ecs/components/AASprite.h"
#include "Kikan/renderer/stdRenderer/buffers/Texture2D.h"
#include "systems/TriggerSystem.h"
#include "util/ResourceManager.h"
#include "util/AnimationManager.h"
#include "systems/PlayerAnimationSystem.h"
#include "systems/PlayerStateSystem.h"
#include "systems/AttackAnimationSystem.h"
#include "Kikan/core/Timer.h"
#include "systems/HealthbarSystem.h"
#include "systems/BgSpriteSystem.h"
#include "components/BgSprite.h"
#include "systems/ResourceLoadingSystem.h"

#include <Kikan/ui/elements/Label.h>

void addUI() {
    Kikan::Engine* engine = Kikan::Engine::Kikan();
    auto* ui = engine->getUI();
    ui->setDimensions(1600, 900);

    auto* node = new Kikan::UINode("Arena");
    ui->addNode(node);
    auto* healthbar = new Kikan::Label("Healthbar", glm::vec2(20, 120), glm::vec2(5780 / 960 * 100, 100), nullptr);
    ui->addElement(healthbar, node);
    auto* attack = new Kikan::Label("Attack", glm::vec2(1600 - 140, 120), glm::vec2(100), nullptr);
    ui->addElement(attack, node);
    auto* ability = new Kikan::Label("Ability", glm::vec2(1600 - 260, 120), glm::vec2(100), nullptr);
    ui->addElement(ability, node);
#ifdef ENABLE_ULT
    auto* ultimate = new Kikan::Label("Ultimate", glm::vec2(1600 - 380, 120), glm::vec2(100), nullptr);
    ui->addElement(ultimate, node);
#endif
    auto* attackGray = new Kikan::Label("AttackG", glm::vec2(1600 - 140, 120), glm::vec2(100), nullptr);
    attackGray->setTextureLayerOffset(attackGray->getTextureLayerOffset() - 0.01f);
    ui->addElement(attackGray, node);
    auto* abilityGray = new Kikan::Label("AbilityG", glm::vec2(1600 - 260, 120), glm::vec2(100), nullptr);
    abilityGray->setTextureLayerOffset(abilityGray->getTextureLayerOffset() - 0.01f);
    ui->addElement(abilityGray, node);
#ifdef ENABLE_ULT
    auto* ultimateGray = new Kikan::Label("UltimateG", glm::vec2(1600 - 380, 120), glm::vec2(100), nullptr);
    ultimateGray->setTextureLayerOffset(ultimateGray->getTextureLayerOffset() - 0.01f);
    ui->addElement(ultimateGray, node);
#endif
}

void addBoundaries(){
    Kikan::Engine* engine = Kikan::Engine::Kikan();
    // Ground
    {
        auto *entity = new Kikan::Entity();
        entity->getComponent<Kikan::Transform>()->position = glm::vec3(0, 81, 0);

        auto *sprite = new Kikan::LineQuadSprite();
        sprite->dimensions = glm::vec2(1000, 81);
        sprite->color = glm::vec4(.4, .4, .4, 1);
        sprite->thickness = 50;
        sprite->layer = -.2;
        entity->addComponent(sprite);

        auto *collider = new SColliderComponent();
        collider->dimensions = glm::vec2(1000, 81);
        entity->addComponent(collider);

        engine->getECS()->getScene()->addEntity(entity);
    }
    // Left Wall
    {
        auto *entity = new Kikan::Entity();
        entity->getComponent<Kikan::Transform>()->position = glm::vec3(1000, 1000, 0);

        auto *sprite = new Kikan::LineQuadSprite();
        sprite->dimensions = glm::vec2(20, 1000);
        sprite->color = glm::vec4(.4, .4, .4, 1);
        sprite->thickness = 5;
        entity->addComponent(sprite);

        auto *collider = new SColliderComponent();
        collider->dimensions = glm::vec2(20, 1000);
        entity->addComponent(collider);

        engine->getECS()->getScene()->addEntity(entity);
    }
    // Right Wall
    {
        auto *entity = new Kikan::Entity();
        entity->getComponent<Kikan::Transform>()->position = glm::vec3(-20, 1000, 0);

        auto *sprite = new Kikan::LineQuadSprite();
        sprite->dimensions = glm::vec2(20, 1000);
        sprite->color = glm::vec4(.4, .4, .4, 1);
        sprite->thickness = 5;
        entity->addComponent(sprite);

        auto *collider = new SColliderComponent();
        collider->dimensions = glm::vec2(20, 1000);
        entity->addComponent(collider);

        engine->getECS()->getScene()->addEntity(entity);
    }
}

int WinMain() {
    Kikan::Engine::init();
    Kikan::Engine* engine = Kikan::Engine::Kikan();

    Resource::init();

    auto* resLoadingSystem = new ResourceLoadingSystem();
    auto* cameraSystem = new CameraSystem();
    auto* physicsSystem = new PhysicsSystem();
    physicsSystem->gravity = GRAVITY;
    auto* effectSystem = new EffectSystem();
    auto* movSystem = new PlayerMovementSystem();
    auto* clientSystem = new NetworkingClientSystem();
    auto* triggerSystem = new TriggerSystem();
    auto* collisionSystem = new CollisionSystem();
    auto* playerStateSystem = new PlayerStateSystem();
    auto* playerAnimationSystem = new PlayerAnimationSystem();
    auto* attackAnimationSystem = new AttackAnimationSystem();
    auto* healthbarSystem = new HealthbarSystem();
    auto* bgSpriteSystem = new BgSpriteSystem();

    engine->getECS()->getScene()->addSystem(resLoadingSystem);
    engine->getECS()->getScene()->addSystem(cameraSystem);
    engine->getECS()->getScene()->addSystem(physicsSystem);
    engine->getECS()->getScene()->addSystem(effectSystem);
    engine->getECS()->getScene()->addSystem(movSystem);
    engine->getECS()->getScene()->addSystem(clientSystem);
    engine->getECS()->getScene()->addSystem(triggerSystem);
    engine->getECS()->getScene()->addSystem(collisionSystem);
    engine->getECS()->getScene()->addSystem(playerStateSystem);
    engine->getECS()->getScene()->addSystem(playerAnimationSystem);
    engine->getECS()->getScene()->addSystem(attackAnimationSystem);
    engine->getECS()->getScene()->addSystem(healthbarSystem);
    engine->getECS()->getScene()->addSystem(bgSpriteSystem);

    auto* spriteSystem = new Kikan::SpriteRenderSystem();

    engine->getECS()->getScene()->addSystem(spriteSystem);

    auto* serverSystem = new NetworkingServerSystem();
    engine->getECS()->createThread(10, 100);
    engine->getECS()->addThreadedSystem(serverSystem, 0);

    {
        auto* entity = new Kikan::Entity;
        auto* sprite = new Kikan::AASprite;
        sprite->offset = glm::vec2(0, 562.5f);
        sprite->dimensions = glm::vec2(3000, 562.5f);
        sprite->textureID = ResourceManager::get<TextureResource>(Resource::ID::TEX_OUTER_WALL_CLOUDS)->getID();
        sprite->layer = .2;
        sprite->color = glm::vec4(.5f);
        entity->addComponent(sprite);
        auto* bgSprite = new BgSprite();
        bgSprite->vel.x = -.01f;
        bgSprite->id = BgSprite::ID::CLOUDS;
        entity->addComponent(bgSprite);
        engine->getECS()->getScene()->addEntity(entity);
    }
    {
        auto* entity = new Kikan::Entity;
        auto* sprite = new Kikan::AASprite;
        sprite->offset = glm::vec2(0, 562.5f);
        sprite->dimensions = glm::vec2(1000, 562.5f);
        sprite->textureID = ResourceManager::get<TextureResource>(Resource::ID::TEX_OUTER_WALL_BACKGROUND)->getID();
        sprite->layer = .1;
        sprite->color = glm::vec4(.5f);
        entity->addComponent(sprite);
        engine->getECS()->getScene()->addEntity(entity);
    }
    {
        auto* entity = new Kikan::Entity;
        auto* sprite = new Kikan::AASprite;
        sprite->offset = glm::vec2(0, 562.5f);
        sprite->dimensions = glm::vec2(1000, 562.5f);
        sprite->textureID = ResourceManager::get<TextureResource>(Resource::ID::TEX_OUTER_WALL_FOREGROUND)->getID();
        sprite->layer = -.1;
        sprite->color = glm::vec4(.5f);
        entity->addComponent(sprite);
        engine->getECS()->getScene()->addEntity(entity);
    }

    // Player
    {
        auto* entity = Spawner::spawnPlayer();
        entity->getComponent<Kikan::Transform>()->position = glm::vec3(500, 800, 0);
        engine->getECS()->getScene()->addEntity(entity);
    }

    addBoundaries();

    addUI();

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