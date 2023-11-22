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

static void CheckEdgeCases(glm::vec2 v, float& mulitplierTop, float& mulitplierBottom, float& mulitplierRight, float& mulitplierLeft)
{
    // if vector goes backwards it's automatically not the right direction
    if (mulitplierTop < 0 && std::isfinite(mulitplierTop)) mulitplierTop = std::numeric_limits<float>::infinity();
    if (mulitplierBottom < 0 && std::isfinite(mulitplierBottom)) mulitplierBottom = std::numeric_limits<float>::infinity();
    if (mulitplierRight < 0 && std::isfinite(mulitplierRight)) mulitplierRight = std::numeric_limits<float>::infinity();
    if (mulitplierLeft < 0 && std::isfinite(mulitplierLeft)) mulitplierLeft = std::numeric_limits<float>::infinity();

    if (v.x == 0)
    {
        mulitplierRight = std::numeric_limits<float>::infinity();
        mulitplierLeft = std::numeric_limits<float>::infinity();
    }

    if (v.y == 0)
    {
        mulitplierTop = std::numeric_limits<float>::infinity();
        mulitplierBottom = std::numeric_limits<float>::infinity();
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

void CollisionSystem::updateColliderPosition(){
    for(auto* entity : _d_entities){
        auto* transform = entity->getComponent<Kikan::Transform>();
        auto* collider = entity->getComponent<DColliderComponent>();
        collider->position = transform->position;

        collider->hasCollidedT = false;
        collider->hasCollidedB = false;
        collider->hasCollidedL = false;
        collider->hasCollidedR = false;
    }
}


void CollisionSystem::updateTransformPosition() {
    for(auto* entity : _d_entities){
        auto* transform = entity->getComponent<Kikan::Transform>();
        auto* collider = entity->getComponent<DColliderComponent>();
        transform->position = glm::vec3(collider->position, transform->position.z);
        collider->lastPosition = collider->position;
    }
}

CollisionSystem::Direction CollisionSystem::collidesDirectionAABB(Kikan::Entity* dEntity, Kikan::Entity* sEntity)
{
    auto* da = dEntity->getComponent<DColliderComponent>();
    auto* tb = sEntity->getComponent<Kikan::Transform>();
    auto* sb = sEntity->getComponent<SColliderComponent>();

    auto v = glm::vec2(da->position.x - da->lastPosition.x, da->position.y - da->lastPosition.y);
    auto lastCenter = glm::vec2();

    // test for top side
    float mulitplierTop = (tb->position.y + sb->dimensions.y + (da->dimensions.y / 2) - lastCenter.y) / v.y;

    // test for bottom side
    float mulitplierBottom = (tb->position.y - (da->dimensions.y / 2) - lastCenter.y) / v.y;

    // test for right side
    float mulitplierRight = (tb->position.x + sb->dimensions.x + (da->dimensions.x / 2) - lastCenter.x) / v.x;

    // test for left side
    float mulitplierLeft = (tb->position.x - (da->dimensions.x / 2) - lastCenter.x) / v.x;

    int zeros = (int(mulitplierTop == 0) * 4) +
                (int(mulitplierLeft == 0) * 4) +
                (int(mulitplierRight == 0) * 3) +
                (int(mulitplierBottom == 0) * 2);
    if (zeros > 6)
        return Direction::UP;
    else if (zeros > 5)
        return Direction::LEFT;
    else if (zeros > 4)
        return Direction::RIGHT;

    CheckEdgeCases(v, mulitplierTop, mulitplierBottom, mulitplierRight, mulitplierLeft);

    if (mulitplierLeft < mulitplierBottom && mulitplierLeft < mulitplierRight && mulitplierLeft < mulitplierTop)
        return Direction::LEFT;
    if (mulitplierRight < mulitplierBottom && mulitplierRight < mulitplierTop && mulitplierRight < mulitplierLeft)
        return Direction::RIGHT;

    if (mulitplierTop < mulitplierBottom && mulitplierTop < mulitplierRight && mulitplierTop < mulitplierLeft)
        return Direction::UP;

    return Direction::DOWN;
}

void CollisionSystem::handleCleanCollision(Direction direction, Kikan::Entity* dEntity, Kikan::Entity* sEntity)
{
    auto* dCollider = dEntity->getComponent<DColliderComponent>();
    auto* sCollider = sEntity->getComponent<SColliderComponent>();
    auto* sTransform = sEntity->getComponent<Kikan::Transform>();

    switch (direction)
    {
        case Direction::DOWN:
            dCollider->hasCollidedT = true;
            dCollider->position = glm::vec2(dCollider->position.x, sTransform->position.y - dCollider->dimensions.y);
            break;
        case Direction::UP:
            dCollider->hasCollidedB = true;
            dCollider->position = glm::vec2(dCollider->position.x, sTransform->position.y + sCollider->dimensions.y);
            break;
        case Direction::LEFT:
            dCollider->hasCollidedR = true;
            dCollider->position = glm::vec2(sTransform->position.x - dCollider->dimensions.x, dCollider->position.y);
            break;
        case Direction::RIGHT:
            dCollider->hasCollidedL = true;
            dCollider->position = glm::vec2(sTransform->position.x + sCollider->dimensions.x, dCollider->position.y);
            break;
    }
}

void CollisionSystem::update(double dt) {
    updateColliderPosition();

    for(auto* dEntity : _d_entities){
        auto* dTransform = dEntity->getComponent<Kikan::Transform>();
        auto* dCollider = dEntity->getComponent<DColliderComponent>();

        for(auto* sEntity : _s_entities){
            auto* sTransform = sEntity->getComponent<Kikan::Transform>();
            auto* sCollider = sEntity->getComponent<SColliderComponent>();

            if(collidedAABB(dCollider->position, dCollider->dimensions, sTransform->position, sCollider->dimensions)){
                Direction dir = collidesDirectionAABB(dEntity, sEntity);
                handleCleanCollision(dir, dEntity, sEntity);
            }
        }
    }

    updateTransformPosition();
}


