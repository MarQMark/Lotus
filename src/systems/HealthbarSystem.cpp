#include "systems/HealthbarSystem.h"
#include "components/HealthbarComponent.h"
#include "Kikan/ecs/components/AASprite.h"
#include "components/HealthComponent.h"
#include "util/ResourceManager.h"

HealthbarSystem::HealthbarSystem() {
    includeSingle(HealthbarComponent);
}

void HealthbarSystem::update(double dt) {
    for(auto* e : _entities){
        auto* sprite = e->getComponent<Kikan::AASprite>();
        auto* health = e->getComponent<HealthComponent>();

        uint32_t id = std::min(std::max(11 - (int)(11 * (health->health / health->maxHealth)), 0), 11);
        ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_HEALTHBAR_ENEMY)->getTexCoords(sprite->texCoords, id);
    }
}
