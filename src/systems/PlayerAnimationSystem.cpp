#include "systems/PlayerAnimationSystem.h"
#include "components/PlayerComponent.h"
#include "components/EnemyComponent.h"
#include "Kikan/ecs/components/Texture2DSprite.h"
#include "Kikan/ecs/components/Physics.h"
#include "components/DColliderComponent.h"
#include "util/AnimationManager.h"

PlayerAnimationSystem::PlayerAnimationSystem() {
    includeSingle(PlayerComponent);
    //includeSingle(EnemyComponent);
}

void PlayerAnimationSystem::update(double dt) {
    for (auto* e : _entities) {
        auto* transform = e->getComponent<Kikan::Transform>();
        auto* player = e->getComponent<PlayerComponent>();
        auto* sprite = e->getComponent<Kikan::Texture2DSprite>();
        auto* physics = e->getComponent<Kikan::Physics>();
        auto* collider = e->getComponent<DColliderComponent>();
        if(!sprite || !physics || !collider)
            return;

        glm::vec2 texCoords[4];
        Animation* animation;
        if(!collider->hasCollidedB){
            if(player->facing == 0)
                animation = AnimationManager::getAnimation(Animation::ID::FIRE_JUMP_RIGHT);
            else
                animation = AnimationManager::getAnimation(Animation::ID::FIRE_JUMP_LEFT);
        }
        else if(physics->velocity.x > 0.2){
            animation = AnimationManager::getAnimation(Animation::ID::FIRE_MOV_RIGHT);
        }
        else if(physics->velocity.x < -0.2){
            animation = AnimationManager::getAnimation(Animation::ID::FIRE_MOV_LEFT);
        }
        else{
            if(player->facing == 0)
                animation = AnimationManager::getAnimation(Animation::ID::FIRE_IDLE_RIGHT);
            else
                animation = AnimationManager::getAnimation(Animation::ID::FIRE_IDLE_LEFT);
        }

        if(_last_animation != animation)
            animation->reset();
        _last_animation = animation;

        animation->getFrame(texCoords);
        for(int i = 0; i < 4; i++)
            sprite->texCoords[i] = texCoords[i];
        sprite->points[0] = glm::vec2(transform->position.x, transform->position.y);
        sprite->points[1] = glm::vec2(transform->position.x + 50, transform->position.y);
        sprite->points[2] = glm::vec2(transform->position.x + 50, transform->position.y - 80);
        sprite->points[3] = glm::vec2(transform->position.x, transform->position.y - 80);

        sprite->textureID = animation->getID();
    }
}
