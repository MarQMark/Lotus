#include "systems/CameraSystem.h"
#include "Kikan/renderer/stdRenderer/StdRenderer.h"
#include "Kikan/Engine.h"

CameraSystem::CameraSystem() {
}

void CameraSystem::update(double dt) {
    auto* renderer = (Kikan::Renderer::StdRenderer*)Kikan::Engine::Kikan()->getRenderer();

    _camera.reset();


    _camera.translate( 0, -1);

    float ratio = (float)renderer->getWidth()/(float)renderer->getHeight();
    _camera.scale( 1.f, ratio);

    // move origin to bottom left
    _camera.translate(-1, 0);

    _camera.scale(2.f/1000.f, 2.f/1000.f);

    renderer->mvp = _camera.matrix();
}
