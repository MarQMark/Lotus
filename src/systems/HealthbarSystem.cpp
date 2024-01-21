#include "systems/HealthbarSystem.h"
#include "components/HealthbarComponent.h"
#include "Kikan/ecs/components/AASprite.h"
#include "components/HealthComponent.h"
#include "components/PlayerStateComponent.h"
#include "components/PlayerComponent.h"
#include "util/ResourceManager.h"
#include "Kikan/Engine.h"
#include "Kikan/ui/elements/Label.h"

HealthbarSystem::HealthbarSystem() {
    includeSingle(HealthbarComponent);
}

void HealthbarSystem::update(double dt) {
    for(auto* e : _entities){
        auto* sprite = e->getComponent<Kikan::AASprite>();
        auto* health = e->getComponent<HealthComponent>();

        uint32_t id = std::min(std::max(10 - (int)(10 * (health->health / health->maxHealth)), 0), 10);
        auto res = ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_HEALTHBAR_ENEMY);
        res->getTexCoords(sprite->texCoords, id);
        sprite->textureID = res->getID();
    }


    auto* player = Kikan::Engine::Kikan()->getECS()->getScene()->getEntity(getSig(PlayerComponent));
    if(!player)
        return;

    auto* playerState = player->getComponent<PlayerStateComponent>();
    auto* playerHealth = player->getComponent<HealthComponent>();
    uint32_t id = std::min(std::max(10 - (int)(10 * (playerHealth->health / playerHealth->maxHealth)), 0), 10);
    SpriteSheetResource* res;
    switch (playerState->nation) {
        case FIRE:
            res = ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_HEALTHBAR_FIRE);
            break;
        case EARTH:
            res = ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_HEALTHBAR_EARTH);
            break;
        case AIR:
            res = ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_HEALTHBAR_AIR);
            break;
#ifdef ENABLE_WATER
        case WATER:
            res = ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_HEALTHBAR_WATER);
            break;
#endif
    }

    auto* ui = Kikan::Engine::Kikan()->getUI();
    auto label = (Kikan::Label*)ui->getElement("Healthbar");
    label->setTexture2D(res->getTexture2D());
    glm::vec2 texCoords[4];
    res->getTexCoords(texCoords, id);
    label->setTextureCoords(texCoords);
}
