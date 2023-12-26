#include "systems/PlayerAnimationSystem.h"
#include "Kikan/ecs/components/AASprite.h"
#include "Kikan/ecs/components/Physics.h"
#include "components/DColliderComponent.h"
#include "util/AnimationManager.h"
#include "components/AnimationComponent.h"
#include "components/PlayerStateComponent.h"
#include "Constants.h"
#include "components/EffectComponent.h"

PlayerAnimationSystem::PlayerAnimationSystem() {
    includeAnd(sig(AnimationComponent), sig(PlayerStateComponent));
}

Animation* getAttackAnimation(PlayerStateComponent* player){
    Animation::ID animationID;
    switch (player->nation) {
        case Nation::FIRE:
            animationID = Animation::ID::FIRE_PLAYER_ATTACK_R;
            break;
        case Nation::EARTH:
            animationID = Animation::ID::EARTH_PLAYER_ATTACK_R;
            break;
        case Nation::AIR:
            animationID = Animation::ID::AIR_PLAYER_ATTACK_R;
            break;
        case Nation::WATER:
            animationID = Animation::ID::WATER_PLAYER_ATTACK_R;
            break;
        default:
            animationID = Animation::ID::FIRE_PLAYER_ATTACK_R;
            break;
    }
    animationID = (Animation::ID)(animationID + player->facing);
    return AnimationManager::getAnimation(animationID);
}
Animation* getJumpAnimation(PlayerStateComponent* player, Kikan::Entity* e){
    Animation::ID animationID;
    switch (player->nation) {
        case Nation::FIRE:
            // TODO: Add ability jump
            animationID = Animation::ID::FIRE_PLAYER_JUMP_R;
            break;
        case Nation::EARTH:
            animationID = Animation::ID::EARTH_PLAYER_JUMP_R;
            break;
        case Nation::AIR:
            animationID = Animation::ID::AIR_PLAYER_JUMP_R;
            break;
        case Nation::WATER:
            animationID = Animation::ID::WATER_PLAYER_JUMP_R;
            break;
        default:
            animationID = Animation::ID::FIRE_PLAYER_JUMP_R;
            break;
    }
    animationID = (Animation::ID)(animationID + player->facing);
    return AnimationManager::getAnimation(animationID);
}
Animation* getMovingAnimation(PlayerStateComponent* player){
    Animation::ID animationID;
    switch (player->nation) {
        case Nation::FIRE:
            animationID = Animation::ID::FIRE_PLAYER_MOV_R;
            break;
        case Nation::EARTH:
            animationID = Animation::ID::EARTH_PLAYER_MOV_R;
            break;
        case Nation::AIR:
            animationID = Animation::ID::AIR_PLAYER_MOV_R;
            break;
        case Nation::WATER:
            animationID = Animation::ID::WATER_PLAYER_MOV_R;
            break;
        default:
            animationID = Animation::ID::FIRE_PLAYER_MOV_R;
            break;
    }
    animationID = (Animation::ID)(animationID + player->facing);
    return AnimationManager::getAnimation(animationID);
}
Animation* getIdleAnimation(PlayerStateComponent* player){
    Animation::ID animationID;
    switch (player->nation) {
        case Nation::FIRE:
            animationID = Animation::ID::FIRE_PLAYER_IDLE_R;
            break;
        case Nation::EARTH:
            animationID = Animation::ID::EARTH_PLAYER_IDLE_R;
            break;
        case Nation::AIR:
            animationID = Animation::ID::AIR_PLAYER_IDLE_R;
            break;
        case Nation::WATER:
            animationID = Animation::ID::WATER_PLAYER_IDLE_R;
            break;
        default:
            animationID = Animation::ID::FIRE_PLAYER_IDLE_R;
            break;
    }
    animationID = (Animation::ID)(animationID + player->facing);
    return AnimationManager::getAnimation(animationID);
}

void PlayerAnimationSystem::update(double dt) {
    for (auto* e : _entities) {
        auto* transform = e->getComponent<Kikan::Transform>();
        auto* sprite = e->getComponent<Kikan::AASprite>();
        auto* animComp =  e->getComponent<AnimationComponent>();
        auto* effect = e->getComponent<EffectComponent>();
        if(!sprite || !effect)
            return;

        auto* player = e->getComponent<PlayerStateComponent>();
        Animation* animation;
        if(effect->effects.count(EffectComponent::ID::ATTACK_CAST))
            animation = getAttackAnimation(player);
        else if(!player->onGround)
            animation = getJumpAnimation(player, e);
        else if(player->isMoving)
            animation = getMovingAnimation(player);
        else
            animation = getIdleAnimation(player);

        // reset if animation changed
        if(animComp->animation != animation){
            animComp->currentFrame = 0;
            animComp->lastTime = std::chrono::high_resolution_clock::now();
        }
        animComp->animation = animation;

        glm::vec2 texCoords[4];
        animation->getFrame(texCoords, animComp->currentFrame, animComp->lastTime);
        for(int i = 0; i < 4; i++)
            sprite->texCoords[i] = texCoords[i];
        sprite->textureID = animation->getID();
    }
}
