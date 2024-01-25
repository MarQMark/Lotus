#include "systems/MenuSystem.h"
#include "Kikan/Engine.h"
#include "scenes/Scenes.h"

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

void MenuSystem::update(double dt) {
    auto* engine = Kikan::Engine::Kikan();
    auto* renderer = (Kikan::StdRenderer*)engine->getRenderer();

    if(_text_init_pos == -1)
        _text_init_pos = engine->getUI()->getElement("start_lbl")->pos.y;

    engine->getUI()->getElement("start_lbl")->pos.y = 20 * sinf(engine->time.tt / 2000) + _text_init_pos;

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

    if(engine->getInput()->keyPressed(Kikan::Key::SPACE) ||
       engine->getInput()->mousePressed(Kikan::Mouse::BUTTON_LEFT)){

        //if(engine->getUI()->getNode(UI_CONNECT_MENU)->enabled)
        //    engine->getECS()->loadScene(SCENE_GAME);

        if(engine->getUI()->getNode(UI_MAIN_MENU)->enabled){
            engine->getUI()->getNode(UI_MAIN_MENU)->enabled = false;
            engine->getUI()->getNode(UI_CONNECT_MENU)->enabled = true;
        }
    }
}
