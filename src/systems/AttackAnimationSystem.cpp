#include "systems/AttackAnimationSystem.h"
#include "components/AnimationComponent.h"
#include "components/DamageComponent.h"
#include "Kikan/ecs/components/AASprite.h"
#include "Kikan/ecs/components/Physics.h"

AttackAnimationSystem::AttackAnimationSystem() {
    includeAnd(sig(AnimationComponent), sig(DamageComponent));
}

void AttackAnimationSystem::update(double dt) {
    for (auto* e : _entities) {
        auto* animComp = e->getComponent<AnimationComponent>();
        auto* sprite = e->getComponent<Kikan::AASprite>();

        glm::vec2 texCoords[4];
        animComp->animation->getFrame(texCoords, animComp->currentFrame, animComp->lastTime);
        for(int i = 0; i < 4; i++)
            sprite->texCoords[i] = texCoords[i];
        sprite->textureID = animComp->animation->getID();
    }
}
