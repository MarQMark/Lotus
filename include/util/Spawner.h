#ifndef LOTUS_SPAWNER_H
#define LOTUS_SPAWNER_H

#include <Kikan/ecs/Entity.h>
#include "Constants.h"

class Spawner {
public:
    static Kikan::Entity* spawnPlayer(Nation nation = Nation::FIRE);
    static Kikan::Entity* spawnEnemy(Nation nation = Nation::FIRE);

    static Kikan::Entity* spawnAttack(Nation nation = Nation::FIRE);

private:
    // Add common components of Player and Enemy
    static void add_pe_common(Kikan::Entity* entity, Nation nation);
};


#endif //LOTUS_SPAWNER_H
