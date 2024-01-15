#include "systems/PlayerAnimationSystem.h"
#include "Kikan/ecs/components/AASprite.h"
#include "Kikan/ecs/components/Physics.h"
#include "components/DColliderComponent.h"
#include "util/AnimationManager.h"
#include "components/AnimationComponent.h"
#include "components/PlayerStateComponent.h"
#include "Constants.h"
#include "components/EffectComponent.h"
#include "Kikan/Engine.h"
#include "components/PlayerComponent.h"
#include "Kikan/ui/elements/Label.h"

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
    }
    animationID = (Animation::ID)(animationID + player->facing);
    return AnimationManager::getAnimation(animationID);
}
Animation* getJumpAnimation(PlayerStateComponent* player, Kikan::Entity* e){
    Animation::ID animationID;
    switch (player->nation) {
        case Nation::FIRE:
            {
                auto* effects = e->getComponent<EffectComponent>();
                if(effects && effects->effects.count(EffectComponent::ID::FIRE_ABILITY)){
                    animationID = Animation::ID::FIRE_PLAYER_JUMP_ABL_R;
                }
                else{
                    animationID = Animation::ID::FIRE_PLAYER_JUMP_R;
                }
            }
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
    }
    animationID = (Animation::ID)(animationID + player->facing);
    return AnimationManager::getAnimation(animationID);
}
Animation* getMovingAnimation(PlayerStateComponent* player, Kikan::Entity* e){
    Animation::ID animationID;
    switch (player->nation) {
        case Nation::FIRE:
            {
                auto* effects = e->getComponent<EffectComponent>();
                if(effects && effects->effects.count(EffectComponent::ID::FIRE_ABILITY)){
                    animationID = Animation::ID::FIRE_PLAYER_JUMP_ABL_R;
                }
                else{
                    animationID = Animation::ID::FIRE_PLAYER_MOV_R;
                }
            }
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
    }
    animationID = (Animation::ID)(animationID + player->facing);
    return AnimationManager::getAnimation(animationID);
}

void updateUICooldowns() {
    auto* player = Kikan::Engine::Kikan()->getECS()->getScene()->getEntity(getSig(PlayerComponent));
    if(!player)
        return;

    auto* playerState = player->getComponent<PlayerStateComponent>();
    auto* effect = player->getComponent<EffectComponent>();
    SpriteSheetResource* res;
    switch (playerState->nation) {
        case FIRE:
            res = ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_UI_COOLDOWN_FIRE);
            break;
        case EARTH:
            res = ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_UI_COOLDOWN_EARTH);
            break;
        case AIR:
            res = ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_UI_COOLDOWN_AIR);
            break;
        case WATER:
            res = ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_UI_COOLDOWN_WATER);
            break;
    }

    auto* ui = Kikan::Engine::Kikan()->getUI();

    {
        auto* label = (Kikan::Label*)ui->getElement("Attack");
        label->setTexture2D(res->getTexture2D());
        glm::vec2 texCoords[4];
        res->getTexCoords(texCoords, 0);
        label->setTextureCoords(texCoords);
    }
    {
        auto* label = (Kikan::Label*)ui->getElement("AttackG");
        label->setTexture2D(res->getTexture2D());
        glm::vec2 texCoords[4];
        res->getTexCoords(texCoords, 1);
        float cooldown = 0;
        if(effect->effects.count(EffectComponent::ID::ATTACK_COOLDOWN))
            cooldown = (float)(effect->effects[EffectComponent::ID::ATTACK_COOLDOWN] / ATTACK_COOLDOWN[playerState->nation]);
        texCoords[2].y += (1 - cooldown) * (texCoords[1].y - texCoords[2].y);
        texCoords[3].y += (1 - cooldown) * (texCoords[0].y - texCoords[3].y);
        label->dim.y = cooldown * 100;
        label->setTextureCoords(texCoords);
    }

    {
        auto* label = (Kikan::Label*)ui->getElement("Ability");
        label->setTexture2D(res->getTexture2D());
        glm::vec2 texCoords[4];
        res->getTexCoords(texCoords, 2);
        label->setTextureCoords(texCoords);
    }
    {
        auto* label = (Kikan::Label*)ui->getElement("AbilityG");
        label->setTexture2D(res->getTexture2D());
        glm::vec2 texCoords[4];
        res->getTexCoords(texCoords, 3);
        float cooldown = 0;
        if(effect->effects.count(EffectComponent::ID::ABILITY_COOLDOWN))
            cooldown = (float)(effect->effects[EffectComponent::ID::ABILITY_COOLDOWN] / ABILITY_COOLDOWN[playerState->nation]);
        texCoords[2].y += (1 - cooldown) * (texCoords[1].y - texCoords[2].y);
        texCoords[3].y += (1 - cooldown) * (texCoords[0].y - texCoords[3].y);
        label->dim.y = cooldown * 100;
        label->setTextureCoords(texCoords);
    }

    {
        auto* label = (Kikan::Label*)ui->getElement("Ultimate");
        label->setTexture2D(res->getTexture2D());
        glm::vec2 texCoords[4];
        res->getTexCoords(texCoords, 4);
        label->setTextureCoords(texCoords);
    }
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
            animation = getMovingAnimation(player, e);
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


    updateUICooldowns();
}
