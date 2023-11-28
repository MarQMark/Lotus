#include "systems/EnemySystem.h"
#include "components/EnemyComponent.h"

EnemySystem::EnemySystem() {
    includeSingle(EnemyComponent);
}

void EnemySystem::update(double dt) {
    for (auto* e : _entities){
        auto* enemy = e->getComponent<EnemyComponent>();
        if(!enemy)
            continue;


    }
}
