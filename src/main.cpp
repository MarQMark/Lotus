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
#include "Kikan/ui/elements/Button.h"
#include "systems/TitleScreenSystem.h"
#include "Kikan/ecs/components/QuadSprite.h"

#include <Kikan/ui/elements/Label.h>

void addGameUI() {
    Kikan::Engine* engine = Kikan::Engine::Kikan();
    auto* ui = engine->getUI();

    auto* node = new Kikan::UINode("Game");
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

    node->enabled = false;
}

void onStartBtnPressed(Kikan::IInteractable* btn, Kikan::IInteractable::State state, void* data){
    //Kikan::Engine::Kikan()->getECS()->loadScene("game");
    Kikan::Engine::Kikan()->getECS()->loadScene("default");
}

void addMenuUI(){
    Kikan::Engine* engine = Kikan::Engine::Kikan();
    auto* ui = engine->getUI();

    auto* node = new Kikan::UINode("MainMenu");
    node->enabled = false;
    ui->addNode(node);
    auto* startBtn = new Kikan::Button("start_btn", glm::vec2(200, 650), glm::vec2(1200, 400));
    startBtn->registerCallback(onStartBtnPressed, Kikan::IInteractable::State::RELEASED);
    ui->addElement(startBtn, node);

}

void addTitleScene(){
    auto* scene = new Kikan::Scene("title");

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


void onMenuSceneLoad(Kikan::Scene* scene, void* data){
    auto* ui = Kikan::Engine::Kikan()->getUI();
    ui->getNode("MainMenu")->enabled = true;
}

void onMenuSceneUnload(Kikan::Scene* scene, void* data){
    auto* ui = Kikan::Engine::Kikan()->getUI();
    ui->getNode("MainMenu")->enabled = false;
}

void addMenuScene(){
    auto* scene = new Kikan::Scene("menu");
    scene->setOnLoad(onMenuSceneLoad);
    scene->setOnUnload(onMenuSceneUnload);

    auto* spriteSystem = new Kikan::SpriteRenderSystem();
    scene->addSystem(spriteSystem);

    Kikan::Engine::Kikan()->getECS()->addScene(scene);
}

void onGameSceneLoad(Kikan::Scene* scene, void* data){
    auto* ui = Kikan::Engine::Kikan()->getUI();
    ui->getNode("Game")->enabled = true;
}

void onGameSceneUnload(Kikan::Scene* scene, void* data){
    auto* ui = Kikan::Engine::Kikan()->getUI();
    ui->getNode("Game")->enabled = false;
}

void addGameScene(){
    auto* scene = Kikan::Engine::Kikan()->getECS()->getScene("default");//new Kikan::Scene("game");
    scene->setOnLoad(onGameSceneLoad);
    scene->setOnUnload(onGameSceneUnload);

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
    scene->addSystem(cameraSystem);
    scene->addSystem(physicsSystem);
    scene->addSystem(effectSystem);
    scene->addSystem(movSystem);
    scene->addSystem(clientSystem);
    scene->addSystem(triggerSystem);
    scene->addSystem(collisionSystem);
    scene->addSystem(playerStateSystem);
    scene->addSystem(playerAnimationSystem);
    scene->addSystem(attackAnimationSystem);
    scene->addSystem(healthbarSystem);
    scene->addSystem(bgSpriteSystem);

    auto* spriteSystem = new Kikan::SpriteRenderSystem();
    scene->addSystem(spriteSystem);

    //Kikan::Engine::Kikan()->getECS()->addScene(scene);
}

int WinMain() {
    Kikan::Engine::init();
    Kikan::Engine* engine = Kikan::Engine::Kikan();

    Resource::init();

    auto* resLoadingSystem = new ResourceLoadingSystem();
    engine->getECS()->addSystem(resLoadingSystem);

    addTitleScene();
    addMenuScene();
    addGameScene();

    auto* serverSystem = new NetworkingServerSystem();
    engine->getECS()->createThread(10, 100);
    engine->getECS()->addThreadedSystem(serverSystem, 0);

    MapManager::add("default", new Map());
    MapManager::get("default")->load(engine->getECS()->getScene("game"));

    // Player
    {
        auto* entity = Spawner::spawnPlayer();
        entity->getComponent<Kikan::Transform>()->position = glm::vec3(500, 800, 0);
        engine->getECS()->getScene()->addEntity(entity);
    }

    engine->getUI()->setDimensions(1600, 900);
    addMenuUI();
    addGameUI();


#ifdef DEBUG
    engine->getECS()->loadScene("menu");
#else
    engine->getECS()->loadScene("title");
#endif

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