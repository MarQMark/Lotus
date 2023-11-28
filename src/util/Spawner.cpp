#include "util/Spawner.h"
#include "Kikan/ecs/components/Physics.h"
#include "Kikan/ecs/components/LineQuadSprite.h"
#include "components/DColliderComponent.h"
#include "components/PlayerComponent.h"
#include "components/EnemyComponent.h"

Kikan::Entity *Spawner::spawnPlayer(Nation nation) {
    auto* entity = new Kikan::Entity;

    auto* physics = new Kikan::Physics();
    physics->friction.x = PLAYER_FRICTION;
    entity->addComponent(physics);

    add_pe_common(entity);

    entity->getComponent<Kikan::LineQuadSprite>()->color = glm::vec4(.4, .5, .8, 1);

    auto* player = new PlayerComponent();
    entity->addComponent(player);

    return entity;
}

Kikan::Entity *Spawner::spawnEnemy(Nation nation) {
    auto* entity = new Kikan::Entity;

    auto* enemyComponent = new EnemyComponent();
    entity->addComponent(enemyComponent);

    add_pe_common(entity);

    entity->getComponent<Kikan::LineQuadSprite>()->color = glm::vec4(.8, .5, .4, 1);

    return entity;
}

Kikan::Entity *Spawner::spawnAttack(Nation nation) {
    auto* entity = new Kikan::Entity;

    return entity;
}

void Spawner::add_pe_common(Kikan::Entity *entity) {
    auto* sprite = new Kikan::LineQuadSprite();
    sprite->dimensions = glm::vec2(50, 80);
    sprite->thickness = 3;
    entity->addComponent(sprite);

    auto* collider = new DColliderComponent();
    collider->dimensions = glm::vec2(50, 80);
    entity->addComponent(collider);
}
