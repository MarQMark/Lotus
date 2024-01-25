#include "scenes/Scenes.h"
#include "Kikan/ui/elements/IInteractable.h"
#include "Kikan/Engine.h"
#include "Kikan/ui/elements/Button.h"
#include "Kikan/ecs/systems/SpriteRenderSystem.h"
#include "systems/MenuSystem.h"
#include "shaders/MainMenuShaders.h"
#include "Kikan/ui/elements/Label.h"
#include "Kikan/ui/elements/Textbox.h"

void onStartBtnPressed(Kikan::IInteractable* btn, Kikan::IInteractable::State state, void* data){
    Kikan::Engine::Kikan()->getECS()->loadScene(SCENE_GAME);
}

void addMenuUI(){
    Kikan::Engine* engine = Kikan::Engine::Kikan();
    auto* ui = engine->getUI();

    auto* node = new Kikan::UINode(UI_MAIN_MENU);
    node->enabled = false;
    ui->addNode(node);


    auto* startLbl = new Kikan::Label("start_lbl", glm::vec2(480, 350), glm::vec2(1200, 50),"Press [SPACE] to start");
    startLbl->setBgColor(glm::vec4(0));
    ui->addElement(startLbl, node);


    node = new Kikan::UINode(UI_CONNECT_MENU);
    node->enabled = false;
    ui->addNode(node);

    auto* nameTxt = new Kikan::Textbox("connect_name", glm::vec2(100, 800), glm::vec2(1000, 100));
    nameTxt->setTooltip("Name");
    nameTxt->setTooltipColor(glm::vec4(.8));
    nameTxt->setOutlineThickness(1);
    ui->addElement(nameTxt, node);

    auto* ipTxt = new Kikan::Textbox("connect_ip", glm::vec2(100, 600), glm::vec2(1000, 100));
    ipTxt->setTooltip("IP Address");
    ipTxt->setTooltipColor(glm::vec4(.8));
    ipTxt->setOutlineThickness(1);
    ui->addElement(ipTxt, node);

    auto* startBtn = new Kikan::Button("connect_btn", glm::vec2(1100, 280), glm::vec2(400, 100));
    startBtn->registerCallback(onStartBtnPressed, Kikan::IInteractable::State::RELEASED);
    ui->addElement(startBtn, node);

    auto* connectLbl = new Kikan::Label("connect_lbl", glm::vec2(1100, 255), glm::vec2(400, 50), " Host");
    connectLbl->setFontLayerOffset(-.2);
    connectLbl->setBgColor(glm::vec4(0));
    ui->addElement(connectLbl, node);
}

void onMenuSceneLoad(Kikan::Scene* scene, void* data){
    auto* ui = Kikan::Engine::Kikan()->getUI();
    ui->getNode(UI_MAIN_MENU)->enabled = true;
}

void onMenuSceneUnload(Kikan::Scene* scene, void* data){
    auto* ui = Kikan::Engine::Kikan()->getUI();
    ui->getNode(UI_CONNECT_MENU)->enabled = false;
}

void addMenuScene(){
    auto* scene = new Kikan::Scene(SCENE_MENU);
    scene->setOnLoad(onMenuSceneLoad);
    scene->setOnUnload(onMenuSceneUnload);

    auto* spriteSystem = new Kikan::SpriteRenderSystem();
    scene->addSystem(spriteSystem);

    auto* menuSystem = new MenuSystem();
    scene->addSystem(menuSystem);

    ((Kikan::StdRenderer*)Kikan::Engine::Kikan()->getRenderer())->shader(new Kikan::Shader(MAIN_MENU_VERT, MAIN_MENU_FRAG), "MainMenu");

    Kikan::Engine::Kikan()->getECS()->addScene(scene);
}