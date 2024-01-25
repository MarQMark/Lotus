#include "Kikan/Engine.h"
#include "util/Spawner.h"
#include "util/ResourceManager.h"
#include "systems/ResourceLoadingSystem.h"

#include "scenes/Scenes.h"

int WinMain() {
    Kikan::Engine::init();
    Kikan::Engine* engine = Kikan::Engine::Kikan();

    Resource::init();

    auto* resLoadingSystem = new ResourceLoadingSystem();
    engine->getECS()->addSystem(resLoadingSystem);

    addTitleScene();
    addMenuScene();
    addGameScene();

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