#include "systems/TitleScreenSystem.h"
#include "Kikan/Engine.h"
#include "Kikan/ecs/components/QuadSprite.h"

TitleScreenSystem::TitleScreenSystem() {
    includeSingle(Kikan::QuadSprite);
}

void TitleScreenSystem::update(double dt) {
    _duration -= dt;

    for(auto* e : _entities){
        auto* sprite = e->getComponent<Kikan::QuadSprite>();

        if(_duration > _max_duration - _fade_in)
            sprite->color.w = 1 - (_max_duration - _duration) / _fade_in;
        else if(_duration < _fade_out)
            sprite->color.w = 1 - _duration / _fade_out;
    }

    if(_duration <= 0)
        Kikan::Engine::Kikan()->getECS()->loadScene("menu");
}
