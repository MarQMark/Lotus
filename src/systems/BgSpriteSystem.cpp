#include "systems/BgSpriteSystem.h"
#include "components/BgSprite.h"

BgSpriteSystem::BgSpriteSystem() {
    includeSingle(BgSprite);
}

void BgSpriteSystem::update(double dt) {
    for (auto* e : _entities) {
        auto bgSprite = e->getComponent<BgSprite>();
        switch (bgSprite->id) {
            case BgSprite::ID::CLOUDS:
                {
                    auto* transform = e->getComponent<Kikan::Transform>();
                    transform->position.x += bgSprite->vel.x * dt;
                    if(transform->position.x < -1800)
                        transform->position.x = 0;
                }
                break;
            case BgSprite::ID::TRAIN:
                break;
        }
    }
}
