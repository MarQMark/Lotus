#include <random>
#include "util/Spawner.h"
#include "Kikan/ecs/components/Physics.h"
#include "Kikan/ecs/components/LineQuadSprite.h"
#include "components/DColliderComponent.h"
#include "components/PlayerComponent.h"
#include "components/EnemyComponent.h"
#include "components/TriggerComponent.h"
#include "components/EffectComponent.h"
#include "Kikan/ecs/components/AASprite.h"
#include "components/AnimationComponent.h"
#include "components/PlayerStateComponent.h"
#include "components/DamageComponent.h"
#include "Kikan/Engine.h"
#include "components/HealthbarComponent.h"
#include "components/HealthComponent.h"
#include "util/GameState.h"


Kikan::Entity *Spawner::spawnPlayer(Nation nation, bool isEnemy) {
    auto* entity = new Kikan::Entity;

    auto* physics = new Kikan::Physics();
    physics->friction.x = PLAYER_FRICTION;
    entity->addComponent(physics);

    add_pe_common(entity, nation);

#ifdef DEBUG
    entity->getComponent<Kikan::LineQuadSprite>()->color = glm::vec4(.4, .5, .8, 1);
#endif
    entity->getComponent<Kikan::AASprite>()->layer = -0.01;

    auto* player = new PlayerComponent();
    entity->addComponent(player);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(1000, 9999);
    //todo make random Id
    entity->getComponent<PlayerStateComponent>()->playerID = distribution(gen);
    entity->getComponent<PlayerStateComponent>()->isEnemy = isEnemy;

    GameState::getInstance().setPlayerComponent(entity->getComponent<PlayerStateComponent>()->playerID);
    GameState::getInstance().setPlayerInput(entity->getComponent<PlayerStateComponent>()->playerID, 0);


    return entity;
}

Kikan::Entity *Spawner::spawnEnemy(Nation nation) {
    auto* entity = new Kikan::Entity;

    auto* physics = new Kikan::Physics();
    physics->friction.x = PLAYER_FRICTION;
    entity->addComponent(physics);

    add_pe_common(entity, nation);

    auto* enemyComponent = new EnemyComponent();
    entity->addComponent(enemyComponent);


#ifdef DEBUG
    entity->getComponent<Kikan::LineQuadSprite>()->color = glm::vec4(.8, .5, .4, 1);
#endif

    return entity;
}

Kikan::Entity *Spawner::spawnAttack(glm::vec2 pos, Nation nation, uint8_t dir) {
    auto* entity = new Kikan::Entity;

#ifdef DEBUG
    auto* hitboxSprite = new Kikan::LineQuadSprite();
#endif
    auto* trigger = new TriggerComponent();
    auto* physics = new Kikan::Physics();
    auto* animComp = new AnimationComponent();
    auto* damage = new DamageComponent();
    auto* sprite = new Kikan::AASprite();

    trigger->impulse = glm::vec2(0.15, 0);
    physics->gravity = false;

    double aspectRatio;
    uint32_t height;
    float width;
    Animation::ID animationID;
    switch (nation) {
        case Nation::EARTH:
            aspectRatio = 340. / 180.;
            animationID = Animation::ID::EARTH_ATTACK_R;
            break;
        case Nation::AIR:
            aspectRatio = 195. / 485.;
            animationID = Animation::ID::AIR_ATTACK_R;
            break;
#ifdef ENABLE_WATER
        case Nation::WATER:
#endif
        case Nation::FIRE:
        default:
            aspectRatio = 300. / 150.;
            animationID = Animation::ID::FIRE_ATTACK_R;
            break;
    }
    height = ATK_SPRITE_HEIGHT[nation];
    damage->damage = ATK_DAMAGE[nation];
    physics->velocity.x = ATTACK_VEL[nation];
    width = (float)(height * aspectRatio);
#ifdef DEBUG
    hitboxSprite->dimensions = glm::vec2(width, height);
    hitboxSprite->thickness = 10;
    hitboxSprite->color = glm::vec4 (0);
#endif
    trigger->dimensions = glm::vec2(width, height);
    sprite->dimensions = glm::vec2(width, height);
    sprite->color = glm::vec4(0);

    if(dir == 1){
        physics->velocity.x *= -1;
        trigger->impulse.x *= -1;
        animationID = (Animation::ID)(animationID + 1);
        entity->getComponent<Kikan::Transform>()->position.x = pos.x - width - 10;
    }
    else{
        entity->getComponent<Kikan::Transform>()->position.x = pos.x + PLAYER_WIDTH + 10;
    }
    entity->getComponent<Kikan::Transform>()->position.y = pos.y - 20;

    animComp->animation = AnimationManager::getAnimation(animationID);

#ifdef DEBUG
    entity->addComponent(hitboxSprite);
#endif
    entity->addComponent(trigger);
    entity->addComponent(physics);
    entity->addComponent(animComp);
    entity->addComponent(damage);
    entity->addComponent(sprite);

    return entity;
}

void Spawner::add_pe_common(Kikan::Entity *entity, Nation nation) {
#ifdef DEBUG
    auto* sprite = new Kikan::LineQuadSprite();
    sprite->dimensions = glm::vec2(PLAYER_WIDTH, PLAYER_HEIGHT);
    sprite->thickness = 3;
    entity->addComponent(sprite);
#endif

    auto* collider = new DColliderComponent();
    collider->dimensions = glm::vec2(PLAYER_WIDTH, PLAYER_HEIGHT);
    entity->addComponent(collider);

    auto* texture = new Kikan::AASprite;
    texture->dimensions = glm::vec2(PLAYER_WIDTH, PLAYER_HEIGHT);
    texture->layer = 0;
    texture->color = glm::vec4(1.f);
    entity->addComponent(texture);

    auto* playerState = new PlayerStateComponent();
    playerState->nation = nation;
    entity->addComponent(playerState);

    auto* animator = new AnimationComponent();
    entity->addComponent(animator);

    auto* effect = new EffectComponent();
    entity->addComponent(effect);

    auto* health = new HealthComponent();
    entity->addComponent(health);

    add_healthbar(entity);
}

void Spawner::add_healthbar(Kikan::Entity* player){
    auto* entity = new Kikan::Entity;
    auto* sprite = new Kikan::AASprite();
    sprite->dimensions = glm::vec2(10 * 400.f / 50.f, 10);
    sprite->offset.y = 20;
    sprite->offset.x = -(sprite->dimensions.x - PLAYER_WIDTH) / 2.f;
    auto ss = ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_HEALTHBAR_ENEMY);
    ss->getTexCoords(sprite->texCoords, 0);
    sprite->textureID = ss->getID();

    auto* healthbarComp = new HealthbarComponent();
    entity->addComponent(healthbarComp);

    // This probably will cause a crash at some point and is generally pretty bad. Too bad!
    delete entity->getComponent<Kikan::Transform>();
    entity->addComponent(player->getComponent<Kikan::Transform>());
    entity->addComponent(player->getComponent<HealthComponent>());

    entity->addComponent(sprite);
    Kikan::Engine::Kikan()->getECS()->getScene()->addEntity(entity);
}

Kikan::Entity *Spawner::spawnEarthAbility(glm::vec2 pos, uint8_t dir) {
    auto* entity = new Kikan::Entity;

    auto* res = ResourceManager::get<TextureResource>(Resource::ID::TEX_EARTH_ABILITY);
    float w = EARTH_ABL_SPRITE_WIDTH;
    float h = EARTH_ABL_SPRITE_WIDTH * ((float)res->getHeight() / (float)res->getWidth());

    auto* transform = entity->getComponent<Kikan::Transform>();
    transform->position.y = pos.y - PLAYER_HEIGHT;
    if(dir == 1){
        transform->position.x = pos.x - w - PLAYER_WIDTH / 2.f;
    }
    else{
        transform->position.x = pos.x + PLAYER_WIDTH * 1.5f;
    }

    auto* texture = new Kikan::AASprite;
    texture->dimensions = glm::vec2(w, h);
    texture->layer = -0.1f;
    texture->textureID = res->getID();
    texture->color = glm::vec4(1.f);
    entity->addComponent(texture);

    if(dir == 1){
        texture->texCoords[0] = glm::vec2 (1, 1);
        texture->texCoords[1] = glm::vec2 (0, 1);
        texture->texCoords[2] = glm::vec2 (0, 0);
        texture->texCoords[3] = glm::vec2 (1, 0);
    }

    float speed = .1f;
    auto* effect = new EffectComponent();
    effect->effects[EffectComponent::ID::SELF_DESTRUCT] = h / speed;
    entity->addComponent(effect);

    auto* physics = new Kikan::Physics();
    physics->gravity = false;
    physics->velocity.y = speed;
    entity->addComponent(physics);

    auto* trigger = new TriggerComponent();
    trigger->dimensions.x = w;
    trigger->dimensions.y = h;
    trigger->triggerStatic = false;
    trigger->impulse.y = EARTH_ABL_IMPULSE;
    entity->addComponent(trigger);

    auto* damage = new DamageComponent();
    damage->damage = EARTH_ABL_DAMAGE;
    entity->addComponent(damage);

    return entity;
}
