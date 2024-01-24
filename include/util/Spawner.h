#ifndef LOTUS_SPAWNER_H
#define LOTUS_SPAWNER_H

#include <Kikan/ecs/Entity.h>
#include "Constants.h"

class Spawner {
public:
    static Kikan::Entity* spawnPlayer(Nation nation = Nation::FIRE, bool isEnemy = false);
    static Kikan::Entity* spawnEnemy(Nation nation = Nation::FIRE);

    static Kikan::Entity* spawnAttack(glm::vec2 pos, Nation nation = Nation::FIRE, uint8_t dir = 0);
    static Kikan::Entity* spawnEarthAbility(glm::vec2 pos, uint8_t dir = 0);

private:
    // Add common components of Player and Enemy
    static void add_pe_common(Kikan::Entity* entity, Nation nation);
    static void add_healthbar(Kikan::Entity* player);
};


#endif //LOTUS_SPAWNER_H
