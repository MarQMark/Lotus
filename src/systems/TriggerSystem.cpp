#include "systems/TriggerSystem.h"
#include "components/TriggerComponent.h"
#include "components/SColliderComponent.h"
#include "components/DColliderComponent.h"
#include "Kikan/Engine.h"
#include "Kikan/core/Logging.h"
#include "components/PlayerComponent.h"
#include "components/HealthComponent.h"
#include "Kikan/ecs/components/Physics.h"
#include "components/DamageComponent.h"

TriggerSystem::TriggerSystem() {
    includeSingle(TriggerComponent);
    includeSingle(SColliderComponent);
    includeSingle(DColliderComponent);
}

static bool collidedAABB(glm::vec2 pos1, glm::vec2 dim1, glm::vec2 pos2, glm::vec2 dim2){
    return
            pos1.x < pos2.x + dim2.x &&
            pos1.x + dim1.x > pos2.x &&
            pos1.y < pos2.y + dim2.y &&
            pos1.y + dim1.y > pos2.y;
}

void TriggerSystem::addEntity(Kikan::Entity *entity) {
    ISystem::addEntity(entity);

    if(entity->getComponent<TriggerComponent>())
        _t_entities.push_back(entity);
    if(entity->getComponent<SColliderComponent>())
        _s_entities.push_back(entity);
    if(entity->getComponent<DColliderComponent>())
        _d_entities.push_back(entity);
}

void TriggerSystem::removeEntity(Kikan::Entity *entity) {
    ISystem::removeEntity(entity);

    if(entity->getComponent<TriggerComponent>()){
        for (int i = 0; i < (int)_t_entities.size(); ++i) {
            if(_t_entities.at(i) == entity){
                _t_entities.erase(_t_entities.begin() + i);
                return;
            }
        }
    }

    if(entity->getComponent<SColliderComponent>()){
        for (int i = 0; i < (int)_s_entities.size(); ++i) {
            if(_s_entities.at(i) == entity){
                _s_entities.erase(_s_entities.begin() + i);
                return;
            }
        }
    }

    if(entity->getComponent<DColliderComponent>()){
        for (int i = 0; i < (int)_d_entities.size(); ++i) {
            if(_d_entities.at(i) == entity){
                _d_entities.erase(_d_entities.begin() + i);
                return;
            }
        }
    }
}

void TriggerSystem::update(double dt) {
    for (size_t i = 0; i < _t_entities.size(); i++) {
        auto* entity = _t_entities[i];
        auto* transform = entity->getComponent<Kikan::Transform>();
        auto* trigger = entity->getComponent<TriggerComponent>();

        if(trigger->triggerStatic){
            for (auto* scEntity : _s_entities) {
                auto* scTransform = scEntity->getComponent<Kikan::Transform>();
                auto* scCollider = scEntity->getComponent<SColliderComponent>();

                if(collidedAABB(
                        glm::vec2(transform->position.x, transform->position.y) + trigger->offset - glm::vec2(0, trigger->dimensions.y),
                        trigger->dimensions,
                        glm::vec2(scTransform->position.x, scTransform->position.y) + scCollider->offset - glm::vec2(0, scCollider->dimensions.y),
                        scCollider->dimensions
                )){
                    i--;
                    Kikan::Engine::Kikan()->getECS()->getScene()->removeEntity(entity);
                    continue;
                }
            }
        }

        for (auto* scEntity : _d_entities) {
            auto* dcTransform = scEntity->getComponent<Kikan::Transform>();
            auto* dcCollider = scEntity->getComponent<DColliderComponent>();

            if(collidedAABB(
                    glm::vec2(transform->position.x, transform->position.y) + trigger->offset - glm::vec2(0, trigger->dimensions.y),
                    trigger->dimensions,
                    glm::vec2(dcTransform->position.x, dcTransform->position.y) + dcCollider->offset - glm::vec2(0, dcCollider->dimensions.y),
                    dcCollider->dimensions
            )){
                auto* player = Kikan::Engine::Kikan()->getECS()->getScene()->getEntity(getSig(PlayerComponent));
                if(scEntity == player){
                    auto* physics = player->getComponent<Kikan::Physics>();
                    physics->acceleration += trigger->impulse;
                }

                // Add knockback
                /*auto* physics = scEntity->getComponent<Kikan::Physics>();
                if(physics)
                    physics->acceleration += trigger->impulse;*/

                // Add damage
                auto* health = scEntity->getComponent<HealthComponent>();
                auto* damage = entity->getComponent<DamageComponent>();
                if(health && damage){
                    kikanPrint("Auchie \n");
                    health->health -= damage->damage;
                }


                i--;
                // TODO: Fix Memory Leak
                Kikan::Engine::Kikan()->getECS()->getScene()->deleteEntity(entity);
                continue;
            }
        }
    }
}
