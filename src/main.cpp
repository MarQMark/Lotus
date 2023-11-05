#include "Kikan/Engine.h"
#include "Kikan/renderer/stdRenderer/Camera.h"

int WinMain() {
    Kikan::Engine::init();
    Kikan::Engine* engine = Kikan::Engine::Kikan();

    Kikan::Renderer::Camera camera;
    ((Kikan::Renderer::StdRenderer*)engine->getRenderer())->mvp = camera.matrix();


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