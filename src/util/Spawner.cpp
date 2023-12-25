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
#include "components/PlayerStateComponent.h"
#include "components/DamageComponent.h"

Kikan::Entity *Spawner::spawnPlayer(Nation nation) {
    auto* entity = new Kikan::Entity;

    auto* physics = new Kikan::Physics();
    physics->friction.x = PLAYER_FRICTION;
    entity->addComponent(physics);

    add_pe_common(entity, nation);

    entity->getComponent<Kikan::LineQuadSprite>()->color = glm::vec4(.4, .5, .8, 1);
    entity->getComponent<Kikan::Texture2DSprite>()->layer = -0.01;

    auto* player = new PlayerComponent();
    entity->addComponent(player);

    auto* effect = new EffectComponent();
    entity->addComponent(effect);

    return entity;
}

Kikan::Entity *Spawner::spawnEnemy(Nation nation) {
    auto* entity = new Kikan::Entity;

    auto* enemyComponent = new EnemyComponent();
    entity->addComponent(enemyComponent);

    add_pe_common(entity, nation);

    entity->getComponent<Kikan::LineQuadSprite>()->color = glm::vec4(.8, .5, .4, 1);

    return entity;
}

Kikan::Entity *Spawner::spawnAttack(Nation nation, uint8_t dir) {
    auto* entity = new Kikan::Entity;

    auto* hitboxSprite = new Kikan::LineQuadSprite();
    hitboxSprite->dimensions = glm::vec2(40, 20);
    hitboxSprite->thickness = 10;
    hitboxSprite->color = glm::vec4 (1.f, 0.f, 0.f, 1.f);
    entity->addComponent(hitboxSprite);

    auto* trigger = new TriggerComponent();
    trigger->dimensions = glm::vec2(40, 20);
    trigger->impulse = glm::vec2(0.15, 0);
    entity->addComponent(trigger);

    auto* physics = new Kikan::Physics();
    physics->gravity = false;
    entity->addComponent(physics);

    auto* animComp = new AnimationComponent();
    auto* damage = new DamageComponent();
    auto* sprite = new Kikan::Texture2DSprite();

    double aspectRatio;
    uint32_t height;
    Animation::ID animationID;
    switch (nation) {
        case Nation::EARTH:
            aspectRatio = 340. / 180.;
            height = EARTH_ATK_SPRITE_HEIGHT;
            animationID = Animation::ID::EARTH_ATTACK_R;
            damage->damage = EARTH_ATK_DAMAGE;
            physics->velocity.x = EARTH_ATTACK_VEL;
            break;
        case Nation::AIR:
            aspectRatio = 195. / 485.;
            height = AIR_ATK_SPRITE_HEIGHT;
            animationID = Animation::ID::AIR_ATTACK_R;
            damage->damage = AIR_ATK_DAMAGE;
            physics->velocity.x = AIR_ATTACK_VEL;
            break;
        case Nation::WATER:
        case Nation::FIRE:
        default:
            aspectRatio = 300. / 150.;
            height = FIRE_ATK_SPRITE_HEIGHT;
            animationID = Animation::ID::FIRE_ATTACK_R;
            damage->damage = FIRE_ATK_DAMAGE;
            physics->velocity.x = FIRE_ATTACK_VEL;
            break;
    }
    if(dir == 1){
        physics->velocity.x *= -1;
        trigger->impulse.x *= -1;
        animationID = (Animation::ID)(animationID + 1);
    }

    animComp->animation = AnimationManager::getAnimation(animationID);

    entity->addComponent(animComp);
    entity->addComponent(damage);
    entity->addComponent(sprite);

    return entity;
}

void Spawner::add_pe_common(Kikan::Entity *entity, Nation nation) {
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

    auto* playerState = new PlayerStateComponent();
    playerState->nation = nation;
    entity->addComponent(playerState);

    auto* animator = new AnimationComponent();
    entity->addComponent(animator);
}
