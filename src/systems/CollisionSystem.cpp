#include "systems/CollisionSystem.h"
#include "components/DColliderComponent.h"
#include "components/SColliderComponent.h"

#include <limits>

static bool collidedAABB(glm::vec2 pos1, glm::vec2 dim1, glm::vec2 pos2, glm::vec2 dim2){
    return
        pos1.x < pos2.x + dim2.x &&
        pos1.x + dim1.x > pos2.x &&
        pos1.y < pos2.y + dim2.y &&
        pos1.y + dim1.y > pos2.y;
}

static void CheckEdgeCases(glm::vec2 v, float& mTop, float& mBottom, float& mRight, float& mLeft)
{
    // if vector goes backwards it's automatically not the right direction
    if (mTop < 0 && std::isfinite(mTop)) mTop = std::numeric_limits<float>::infinity();
    if (mBottom < 0 && std::isfinite(mBottom)) mBottom = std::numeric_limits<float>::infinity();
    if (mRight < 0 && std::isfinite(mRight)) mRight = std::numeric_limits<float>::infinity();
    if (mLeft < 0 && std::isfinite(mLeft)) mLeft = std::numeric_limits<float>::infinity();

    if (v.x == 0){
        mRight = std::numeric_limits<float>::infinity();
        mLeft = std::numeric_limits<float>::infinity();
    }

    if (v.y == 0){
        mTop = std::numeric_limits<float>::infinity();
        mBottom = std::numeric_limits<float>::infinity();
    }
}


CollisionSystem::CollisionSystem() {
    singleInclude(DColliderComponent);
    singleInclude(SColliderComponent);
}

void CollisionSystem::addEntity(Kikan::Entity *entity) {
    ISystem::addEntity(entity);

    if(entity->getComponent<DColliderComponent>())
        _d_entities.push_back(entity);

    if(entity->getComponent<SColliderComponent>())
        _s_entities.push_back(entity);
}

void CollisionSystem::removeEntity(Kikan::Entity *entity) {
    ISystem::removeEntity(entity);

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

void CollisionSystem::updateColliderPosition(){
    for(auto* entity : _d_entities){
        auto* transform = entity->getComponent<Kikan::Transform>();
        auto* collider = entity->getComponent<DColliderComponent>();
        collider->position = transform->position + glm::vec3(collider->offset, 0) - glm::vec3(0, collider->dimensions.y, 0);

        collider->hasCollidedT = false;
        collider->hasCollidedB = false;
        collider->hasCollidedL = false;
        collider->hasCollidedR = false;
    }

    for(auto* entity : _s_entities) {
        auto *transform = entity->getComponent<Kikan::Transform>();
        auto *collider = entity->getComponent<SColliderComponent>();
        collider->position = transform->position + glm::vec3(collider->offset, 0) - glm::vec3(0, collider->dimensions.y, 0);;
    }
}


void CollisionSystem::updateTransformPosition() {
    for(auto* entity : _d_entities){
        auto* transform = entity->getComponent<Kikan::Transform>();
        auto* collider = entity->getComponent<DColliderComponent>();
        transform->position = glm::vec3(collider->position - collider->offset, transform->position.z) + glm::vec3(0, collider->dimensions.y, 0);;
        collider->lastPosition = collider->position;
    }
}

CollisionSystem::Direction CollisionSystem::collidesDirectionAABB(Kikan::Entity* dEntity, Kikan::Entity* sEntity)
{
    auto* dCollider = dEntity->getComponent<DColliderComponent>();
    auto* sCollider = sEntity->getComponent<SColliderComponent>();

    auto v = dCollider->position - dCollider->lastPosition;
    auto lastCenter = dCollider->lastPosition + glm::vec2(dCollider->dimensions.x, dCollider->dimensions.y) / 2.f;

    float mTop =    (sCollider->position.y + sCollider->dimensions.y + (dCollider->dimensions.y / 2) - lastCenter.y) / v.y;
    float mBottom = (sCollider->position.y - (dCollider->dimensions.y / 2) - lastCenter.y) / v.y;
    float mRight =  (sCollider->position.x + sCollider->dimensions.x + (dCollider->dimensions.x / 2) - lastCenter.x) / v.x;
    float mLeft =   (sCollider->position.x - (dCollider->dimensions.x / 2) - lastCenter.x) / v.x;

    int zeros = (int(mTop == 0) * 4) +
                (int(mLeft == 0) * 4) +
                (int(mRight == 0) * 3) +
                (int(mBottom == 0) * 2);
    if (zeros > 6)
        return Direction::UP;
    else if (zeros > 5)
        return Direction::LEFT;
    else if (zeros > 4)
        return Direction::RIGHT;

    CheckEdgeCases(v, mTop, mBottom, mRight, mLeft);

    if (mLeft < mBottom && mLeft < mRight && mLeft < mTop)
        return Direction::LEFT;
    if (mRight < mBottom && mRight < mTop && mRight < mLeft)
        return Direction::RIGHT;

    if (mTop < mBottom && mTop < mRight && mTop < mLeft)
        return Direction::UP;

    return Direction::DOWN;
}

void CollisionSystem::handleCleanCollision(Direction direction, Kikan::Entity* dEntity, Kikan::Entity* sEntity)
{
    auto* dCollider = dEntity->getComponent<DColliderComponent>();
    auto* sCollider = sEntity->getComponent<SColliderComponent>();

    switch (direction)
    {
        case Direction::DOWN:
            dCollider->hasCollidedT = true;
            dCollider->position.y = sCollider->position.y - dCollider->dimensions.y;
            break;
        case Direction::UP:
            dCollider->hasCollidedB = true;
            dCollider->position.y = sCollider->position.y + sCollider->dimensions.y;
            break;
        case Direction::LEFT:
            dCollider->hasCollidedR = true;
            dCollider->position.x = sCollider->position.x - dCollider->dimensions.x;
            break;
        case Direction::RIGHT:
            dCollider->hasCollidedL = true;
            dCollider->position.x = sCollider->position.x + sCollider->dimensions.x;
            break;
        case Direction::NONE:
            break;
    }
}

void CollisionSystem::update(double dt) {
    updateColliderPosition();

    for(auto* dEntity : _d_entities){
        auto* dCollider = dEntity->getComponent<DColliderComponent>();

        for(auto* sEntity : _s_entities){
            auto* sCollider = sEntity->getComponent<SColliderComponent>();

            if(collidedAABB(dCollider->position, dCollider->dimensions, sCollider->position, sCollider->dimensions)){
                Direction dir = collidesDirectionAABB(dEntity, sEntity);
                handleCleanCollision(dir, dEntity, sEntity);
            }
        }
    }

    updateTransformPosition();
}



