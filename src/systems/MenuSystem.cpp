#include "systems/MenuSystem.h"
#include "Kikan/Engine.h"
#include "scenes/Scenes.h"
#include "Kikan/ui/elements/Textbox.h"
#include "Kikan/ui/elements/Label.h"
#include "Kikan/ui/elements/Button.h"
#include "util/ResourceManager.h"

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

}
