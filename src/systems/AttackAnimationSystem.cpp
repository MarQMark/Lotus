#include "systems/AttackAnimationSystem.h"
#include "components/AnimationComponent.h"
#include "components/DamageComponent.h"
#include "Kikan/ecs/components/Texture2DSprite.h"
#include "Kikan/ecs/components/Physics.h"

AttackAnimationSystem::AttackAnimationSystem() {
    includeAnd(sig(AnimationComponent), sig(DamageComponent));
}

void AttackAnimationSystem::update(double dt) {
    for (auto* e : _entities) {
        auto* transform = e->getComponent<Kikan::Transform>();
        auto* animComp = e->getComponent<AnimationComponent>();
        auto* sprite = e->getComponent<Kikan::Texture2DSprite>();

        glm::vec2 texCoords[4];
        animComp->animation->getFrame(texCoords, animComp->currentFrame, animComp->lastTime);
        for(int i = 0; i < 4; i++)
            sprite->texCoords[i] = texCoords[i];
        sprite->points[0] = glm::vec2(transform->position.x, transform->position.y);
        sprite->points[1] = glm::vec2(transform->position.x + 50, transform->position.y);
        sprite->points[2] = glm::vec2(transform->position.x + 50, transform->position.y - 80);
        sprite->points[3] = glm::vec2(transform->position.x, transform->position.y - 80);
        sprite->textureID = animComp->animation->getID();
    }
}
