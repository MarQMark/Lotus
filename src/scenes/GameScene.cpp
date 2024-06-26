#include "scenes/Scenes.h"

#include "Kikan/ecs/Scene.h"
#include "Kikan/Engine.h"

#include "Constants.h"
#include "systems/CameraSystem.h"
#include "systems/PhysicsSystem.h"
#include "systems/EffectSystem.h"
#include "systems/PlayerMovementSystem.h"
#include "systems/NetworkingClientSystem.h"
#include "systems/TriggerSystem.h"
#include "systems/CollisionSystem.h"
#include "systems/PlayerStateSystem.h"
#include "systems/PlayerAnimationSystem.h"
#include "systems/AttackAnimationSystem.h"
#include "systems/HealthbarSystem.h"
#include "systems/BgSpriteSystem.h"
#include "Kikan/ecs/systems/SpriteRenderSystem.h"
#include "Kikan/ui/elements/Label.h"
#include "map/Map.h"
#include "systems/GameLoopSystem.h"
#include "components/PlayerStateComponent.h"
#include "util/Spawner.h"
#include "components/HealthComponent.h"
#include "components/DamageComponent.h"

void addGameUI() {
    Kikan::Engine* engine = Kikan::Engine::Kikan();
    auto* ui = engine->getUI();

    auto* node = new Kikan::UINode(UI_GAME);
    node->enabled = false;
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

    auto* countdownLbl = new Kikan::Label("Countdown", glm::vec2(450, 600), glm::vec2(0, 250), "Fight");
    countdownLbl->enabled = false;
    auto fontOptions = countdownLbl->getFontOptions();
    fontOptions.color = glm::vec4(0,0,0,1);
    countdownLbl->setFontOptions(fontOptions);
    countdownLbl->setFontLayerOffset(-.5f);
    ui->addElement(countdownLbl, node);
}

void onGameSceneLoad(Kikan::Scene* scene, void* data){
    auto* engine = Kikan::Engine::Kikan();
    auto* ui = engine->getUI();


    std::vector<Kikan::Entity*> players;
    engine->getECS()->getScene(SCENE_GAME)->getEntities(getSig(PlayerStateComponent), &players);

    if(players.size() <= 4){
        auto* enemy = Spawner::spawnEnemy(Nation::EARTH);
        enemy->getComponent<Kikan::Transform>()->position = glm::vec3(100, 800, 0);
        engine->getECS()->getScene(SCENE_GAME)->addEntity(enemy);
        players.push_back(enemy);
    }

    for (auto* player : players) {
        auto* health = player->getComponent<HealthComponent>();
        health->health = health->maxHealth;
    }
    MapManager::get("default")->spawnPlayers(players);

    GameLoopSystem::Countdown = 4000;

    ui->getElement("Countdown")->enabled = true;
    ui->getNode(UI_GAME)->enabled = true;
}

void onGameSceneUnload(Kikan::Scene* scene, void* data){
    auto* engine = Kikan::Engine::Kikan();
    auto* ui = engine->getUI();

    std::vector<Kikan::Entity*> attacks;
    engine->getECS()->getScene()->getEntities(getSig(DamageComponent), &attacks);
    for(auto* attack : attacks)
        engine->getECS()->getScene()->deleteEntity(attack);

    ui->getNode(UI_GAME)->enabled = false;
}

void addGameScene(){
    auto* scene = Kikan::Engine::Kikan()->getECS()->getScene(SCENE_GAME);//new Kikan::Scene(SCENE_GAME);
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
    auto* gameLoopSystem = new GameLoopSystem();
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
    scene->addSystem(gameLoopSystem);

    auto* spriteSystem = new Kikan::SpriteRenderSystem();
    scene->addSystem(spriteSystem);

    MapManager::add("default", new Map());
    MapManager::get("default")->load(scene);

    //Kikan::Engine::Kikan()->getECS()->addScene(scene);
}