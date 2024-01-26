#include "scenes/Scenes.h"
#include "Kikan/ui/elements/IInteractable.h"
#include "Kikan/Engine.h"
#include "Kikan/ui/elements/Button.h"
#include "Kikan/ecs/systems/SpriteRenderSystem.h"
#include "systems/MenuSystem.h"
#include "shaders/MainMenuShaders.h"
#include "Kikan/ui/elements/Label.h"
#include "Kikan/ui/elements/Textbox.h"
#include "components/PlayerComponent.h"
#include "components/PlayerStateComponent.h"

void onConnectBtnReleased(Kikan::IInteractable* btn, Kikan::IInteractable::State state, void* data){
    auto* ui = Kikan::Engine::Kikan()->getUI();
    ui->getNode(UI_CONNECT_MENU)->enabled = false;
    ui->getNode(UI_LOBBY_MENU)->enabled = true;
}

void onNationBtnReleased(Kikan::IInteractable* btn, Kikan::IInteractable::State state, void* data){
    auto* ui = Kikan::Engine::Kikan()->getUI();

    ((Kikan::IInteractable*)ui->getElement("lobby_fire_btn"))->interactable = true;
    ((Kikan::IInteractable*)ui->getElement("lobby_earth_btn"))->interactable = true;
    ((Kikan::IInteractable*)ui->getElement("lobby_air_btn"))->interactable = true;

    btn->interactable = false;

    Nation nation = Nation::FIRE;
    if(btn->getName() == "lobby_fire_btn")
        nation = Nation::FIRE;
    else if(btn->getName() == "lobby_earth_btn")
        nation = Nation::EARTH;
    else if(btn->getName() == "lobby_air_btn")
        nation = Nation::AIR;

    auto* entity = Kikan::Engine::Kikan()->getECS()->getScene()->getEntity(getSig(PlayerComponent));
    if(entity){
        auto* player = entity->getComponent<PlayerStateComponent>();
        if(player)
            player->nation = nation;
    }
}

void onStartGameBtnReleased(Kikan::IInteractable* btn, Kikan::IInteractable::State state, void* data){
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
#ifdef DEBUG
    nameTxt->setText("Player");
#endif
    nameTxt->setTooltipColor(glm::vec4(.8));
    nameTxt->setOutlineThickness(1);
    ui->addElement(nameTxt, node);

    auto* ipTxt = new Kikan::Textbox("connect_ip", glm::vec2(100, 600), glm::vec2(1000, 100));
    ipTxt->setTooltip("IP Address");
    ipTxt->setTooltipColor(glm::vec4(.8));
    ipTxt->setOutlineThickness(1);
    ui->addElement(ipTxt, node);

    auto* connectBtn = new Kikan::Button("connect_btn", glm::vec2(1100, 320), glm::vec2(400, 100));
    connectBtn->registerCallback(onConnectBtnReleased, Kikan::IInteractable::State::RELEASED);
    connectBtn->setColor(glm::vec4(116./255., 38./255., 51./255., 1));
    ui->addElement(connectBtn, node);


    node = new Kikan::UINode(UI_LOBBY_MENU);
    node->enabled = false;
    ui->addNode(node);

    auto* listLbl = new Kikan::Label("lobby_list_lbl", glm::vec2(100, 800), glm::vec2(500, 700), "");
    ui->addElement(listLbl, node);

    auto* fireLbl = new Kikan::Label("lobby_fire_lbl", glm::vec2(775, 750), glm::vec2(200, 369), "");
    ui->addElement(fireLbl, node);
    auto* earthLbl = new Kikan::Label("lobby_earth_lbl", glm::vec2(1025, 750), glm::vec2(200, 369), "");
    ui->addElement(earthLbl, node);
    auto* airLbl = new Kikan::Label("lobby_air_lbl", glm::vec2(1275, 750), glm::vec2(200, 369), "");
    ui->addElement(airLbl, node);

    auto* fireBtn = new Kikan::Button("lobby_fire_btn", glm::vec2(825, 350), glm::vec2(100, 50));
    fireBtn->registerCallback(onNationBtnReleased, Kikan::IInteractable::State::RELEASED);
    fireBtn->setColor(glm::vec4(116./255., 38./255., 51./255., 1));
    fireBtn->setDisabledColor(glm::vec4(0.147843137, 0.025490196, 0.096862745, 1));
    fireBtn->interactable = false;
    ui->addElement(fireBtn, node);
    auto* earthBtn = new Kikan::Button("lobby_earth_btn", glm::vec2(1075, 350), glm::vec2(100, 50));
    earthBtn->registerCallback(onNationBtnReleased, Kikan::IInteractable::State::RELEASED);
    earthBtn->setColor(glm::vec4(116./255., 38./255., 51./255., 1));
    earthBtn->setDisabledColor(glm::vec4(0.147843137, 0.025490196, 0.096862745, 1));
    ui->addElement(earthBtn, node);
    auto* airBtn = new Kikan::Button("lobby_air_btn", glm::vec2(1325, 350), glm::vec2(100, 50));
    airBtn->registerCallback(onNationBtnReleased, Kikan::IInteractable::State::RELEASED);
    airBtn->setColor(glm::vec4(116./255., 38./255., 51./255., 1));
    airBtn->setDisabledColor(glm::vec4(0.147843137, 0.025490196, 0.096862745, 1));
    ui->addElement(airBtn, node);

    auto* startGameBtn = new Kikan::Button("lobby_start_btn", glm::vec2(1100, 220), glm::vec2(400, 100));
    startGameBtn->registerCallback(onStartGameBtnReleased, Kikan::IInteractable::State::RELEASED);
    startGameBtn->setColor(glm::vec4(116./255., 38./255., 51./255., 1));
    startGameBtn->setText(" Start");
    ui->addElement(startGameBtn, node);
}

void onMenuSceneLoad(Kikan::Scene* scene, void* data){
    auto* ui = Kikan::Engine::Kikan()->getUI();
    ui->getNode(UI_MAIN_MENU)->enabled = true;
}

void onMenuSceneUnload(Kikan::Scene* scene, void* data){
    auto* ui = Kikan::Engine::Kikan()->getUI();
    ui->getNode(UI_LOBBY_MENU)->enabled = false;
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