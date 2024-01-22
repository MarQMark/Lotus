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
#include "map/Map.h"

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

    MapManager::add("default", new Map());
    MapManager::get("default")->load(engine->getECS()->getScene());

    // Player
    {
        auto* entity = Spawner::spawnPlayer();
        entity->getComponent<Kikan::Transform>()->position = glm::vec3(500, 800, 0);
        engine->getECS()->getScene()->addEntity(entity);
    }

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