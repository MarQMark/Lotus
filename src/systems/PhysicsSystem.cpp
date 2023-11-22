#include "systems/PhysicsSystem.h"
#include "Kikan/ecs/components/Physics.h"
#include "components/DColliderComponent.h"

PhysicsSystem::PhysicsSystem() {
    singleInclude(Kikan::Physics);
}

void PhysicsSystem::update(double dt) {
    for (auto* e : _entities)
    {
        auto* transform = e->getComponent<Kikan::Transform>();
        auto* physics = e->getComponent<Kikan::Physics>();
        auto* collider = e->getComponent<DColliderComponent>();

        if (collider)
        {
            // If Collided, stop player in that axis
            if ((collider->hasCollidedB && physics->velocity.y < 0.0f) ||
                (collider->hasCollidedT && physics->velocity.y > 0.0f))
                physics->velocity.y = 0;

            if ((collider->hasCollidedL && physics->velocity.x < 0.0f) ||
                (collider->hasCollidedR && physics->velocity.x > 0.0f))
                physics->velocity.x = 0;
        }

        // Add Gravity to acceleration
        physics->acceleration += glm::vec2(0.0f, gravity);

        // Peak downward acceleration
        if (physics->velocity.y <= 0)
            physics->acceleration += glm::vec2 (0.0f, 2.0f * gravity);

        // Friction
        physics->acceleration += glm::vec2(physics->friction.x * physics->velocity.y * physics->friction.x, 0);

        // Physics calculation
        glm::vec2 oldVelocity = physics->velocity;
        physics->velocity += physics->acceleration * (float)dt;
        transform->position += glm::vec3(0.5f * (oldVelocity + physics->velocity) * (float)dt, transform->position.z);

        physics->acceleration = glm::vec2(0);
    }
}
