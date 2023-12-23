#include "util/Spawner.h"
#include "Kikan/ecs/components/Physics.h"
#include "Kikan/ecs/components/LineQuadSprite.h"
#include "components/DColliderComponent.h"
#include "components/PlayerComponent.h"
#include "components/EnemyComponent.h"
#include "components/TriggerComponent.h"
#include "components/EffectComponent.h"
#include "Kikan/ecs/components/Texture2DSprite.h"
#include "components/AnimationComponent.h"

Kikan::Entity *Spawner::spawnPlayer(Nation nation) {
    auto* entity = new Kikan::Entity;

    auto* physics = new Kikan::Physics();
    physics->friction.x = PLAYER_FRICTION;
    entity->addComponent(physics);

    add_pe_common(entity);

    entity->getComponent<Kikan::LineQuadSprite>()->color = glm::vec4(.4, .5, .8, 1);

    auto* player = new PlayerComponent();
    player->nation = nation;
    entity->addComponent(player);

    auto* effect = new EffectComponent();
    entity->addComponent(effect);

    auto* animator = new AnimationComponent();
    entity->addComponent(animator);

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

    auto* sprite = new Kikan::LineQuadSprite();
    sprite->dimensions = glm::vec2(40, 20);
    sprite->thickness = 10;
    sprite->color = glm::vec4 (1.f, 0.f, 0.f, 1.f);
    entity->addComponent(sprite);

    auto* trigger = new TriggerComponent();
    trigger->dimensions = glm::vec2(40, 20);
    trigger->impulse = glm::vec2(0.15, 0);
    entity->addComponent(trigger);

    auto* physics = new Kikan::Physics();
    physics->velocity.x = FIRE_ATTACK_VEL;
    physics->gravity = false;
    entity->addComponent(physics);

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

    auto* texture = new Kikan::Texture2DSprite;
    texture->layer = 0;
    texture->color = glm::vec4(1.f);
    entity->addComponent(texture);
}
