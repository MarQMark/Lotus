#include <sstream>
#include "systems/MenuSystem.h"
#include "Kikan/Engine.h"
#include "scenes/Scenes.h"
#include "Kikan/ui/elements/Textbox.h"
#include "Kikan/ui/elements/Label.h"
#include "Kikan/ui/elements/Button.h"
#include "util/ResourceManager.h"
#include "components/PlayerStateComponent.h"
#include "components/HealthComponent.h"

MenuSystem::MenuSystem() {

}

void menuShaderPreRender(Kikan::AutoBatch* batch, void* data){
     auto* engine = Kikan::Engine::Kikan();
     auto* renderer = (Kikan::StdRenderer*)engine->getRenderer();
     auto* shader = renderer->shader("MainMenu");
     shader->bind();
     shader->uniform2fv("u_resolution", glm::vec2(renderer->getWidth(), renderer->getHeight()));
     shader->uniform1lf("u_time", engine->time.tt / 1000);
}

void MenuSystem::update_main(){
    auto* engine = Kikan::Engine::Kikan();

    if(_text_init_pos == -1)
        _text_init_pos = engine->getUI()->getElement("start_lbl")->pos.y;

    engine->getUI()->getElement("start_lbl")->pos.y = 20 * sinf(engine->time.tt / 2000) + _text_init_pos;

    if(engine->getInput()->keyPressed(Kikan::Key::SPACE) ||
       engine->getInput()->mousePressed(Kikan::Mouse::BUTTON_LEFT)){

        if(engine->getUI()->getNode(UI_MAIN_MENU)->enabled){
            engine->getUI()->getNode(UI_MAIN_MENU)->enabled = false;
            engine->getUI()->getNode(UI_CONNECT_MENU)->enabled = true;
        }
    }
}

void MenuSystem::update_connect(){
    auto* engine = Kikan::Engine::Kikan();

    auto* connectBtn = (Kikan::Button*)engine->getUI()->getElement("connect_btn");
    if(((Kikan::Textbox*)engine->getUI()->getElement("connect_ip"))->getText().empty())
        connectBtn->setText(" Host");
    else
        connectBtn->setText("Connect");

    if(((Kikan::Textbox*)engine->getUI()->getElement("connect_name"))->getText().empty())
        connectBtn->interactable = false;
    else
        connectBtn->interactable = true;
}

void MenuSystem::update_lobby(){
    auto* engine = Kikan::Engine::Kikan();
    glm::vec2 texCoords[4];

    auto* fireLbl = (Kikan::Label*)engine->getUI()->getElement("lobby_fire_lbl");
    auto* fireRes = ResourceManager::get<SpriteSheetResource>(Resource::SS_FIRE_PLAYER);
    fireRes->getTexCoords(texCoords, 0);
    fireLbl->setTexture2D(fireRes->getTexture2D());
    fireLbl->setTextureCoords(texCoords);

    auto* earthLbl = (Kikan::Label*)engine->getUI()->getElement("lobby_earth_lbl");
    auto* earthRes = ResourceManager::get<SpriteSheetResource>(Resource::SS_EARTH_PLAYER);
    earthRes->getTexCoords(texCoords, 0);
    earthLbl->setTexture2D(earthRes->getTexture2D());
    earthLbl->setTextureCoords(texCoords);

    auto* airLbl = (Kikan::Label*)engine->getUI()->getElement("lobby_air_lbl");
    auto* airRes = ResourceManager::get<SpriteSheetResource>(Resource::SS_AIR_PLAYER);
    airRes->getTexCoords(texCoords, 0);
    airLbl->setTexture2D(airRes->getTexture2D());
    airLbl->setTextureCoords(texCoords);

    auto* listLbl = (Kikan::Label*)engine->getUI()->getElement("lobby_list_lbl");
    std::stringstream ss;
    std::vector<Kikan::Entity*> players;
    engine->getECS()->getScene(SCENE_GAME)->getEntities(getSig(PlayerStateComponent), &players);
    for(auto e : players){
        auto* player = e->getComponent<PlayerStateComponent>();
        ss << player->name;
        ss << '\n';
    }
    listLbl->setText(ss.str());
}

void MenuSystem::update_victory(double dt) {
    auto* engine = Kikan::Engine::Kikan();

    if(!_victor){
        std::vector<Kikan::Entity*> players;
        engine->getECS()->getScene(SCENE_GAME)->getEntities(getSig(PlayerStateComponent), &players);

        for (auto* player : players) {
            auto* health = player->getComponent<HealthComponent>();
            if(health && health->health > 0)
                _victor = player;
        }
    }

    Nation nation = Nation::FIRE;
    if(_victor){
        auto* player = _victor->getComponent<PlayerStateComponent>();
        nation = player->nation;

        auto* victorNameLbl = (Kikan::Label*)engine->getUI()->getElement("victory_name_lbl");
        victorNameLbl->setText(player->name);
        auto* victorTextLbl = (Kikan::Label*)engine->getUI()->getElement("victory_text_lbl");
        victorTextLbl->setText("won the Game");
    }
    else{
        auto* victorNameLbl = (Kikan::Label*)engine->getUI()->getElement("victory_name_lbl");
        victorNameLbl->setText("It's a");
        auto* victorTextLbl = (Kikan::Label*)engine->getUI()->getElement("victory_text_lbl");
        victorTextLbl->setText("Draw!");
    }
    glm::vec2 texCoords[4];
    SpriteSheetResource* res;
    switch (nation) {
        case Nation::FIRE:
            res = ResourceManager::get<SpriteSheetResource>(Resource::SS_FIRE_PLAYER);
            break;
        case Nation::EARTH:
            res = ResourceManager::get<SpriteSheetResource>(Resource::SS_EARTH_PLAYER);
            break;
        case Nation::AIR:
            res = ResourceManager::get<SpriteSheetResource>(Resource::SS_AIR_PLAYER);
            break;
    }
    auto* victorLbl = (Kikan::Label*)engine->getUI()->getElement("victory_nation_lbl");
    res->getTexCoords(texCoords, 0);
    victorLbl->setTexture2D(res->getTexture2D());
    victorLbl->setTextureCoords(texCoords);


    _victory_duration -= dt;

    if(_victory_duration < 0){
        _victor = nullptr;
        _victory_duration = 5000;
        engine->getUI()->getNode(UI_VICTORY)->enabled = false;
        engine->getUI()->getNode(UI_LOBBY_MENU)->enabled = true;
    }
}

void MenuSystem::update(double dt) {
    auto* engine = Kikan::Engine::Kikan();
    auto* renderer = (Kikan::StdRenderer*)engine->getRenderer();

    struct Kikan::StdRenderer::Options ops;
    ops.preRender = menuShaderPreRender;
    renderer->renderQuad(
            glm::vec2(-1, 1),
            glm::vec2(1, 1),
            glm::vec2(1, -1),
            glm::vec2(-1, -1),
            glm::vec4(0),
            .5,
            &ops
            );

    if(engine->getUI()->getNode(UI_MAIN_MENU)->enabled)
        update_main();
    else if(engine->getUI()->getNode(UI_CONNECT_MENU)->enabled)
        update_connect();
    else if(engine->getUI()->getNode(UI_LOBBY_MENU)->enabled)
        update_lobby();
    else if(engine->getUI()->getNode(UI_VICTORY)->enabled)
        update_victory(dt);

}

