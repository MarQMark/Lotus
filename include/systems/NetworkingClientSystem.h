#ifndef LOTUS_NETWORKINGCLIENTSYSTEM_H
#define LOTUS_NETWORKINGCLIENTSYSTEM_H

#include "Kikan/ecs/systems/ISystem.h"

class NetworkingClientSystem : public Kikan::ISystem {
public:
    NetworkingClientSystem();

    void update(double dt) override;

    void addEntity(Kikan::Entity *entity) override;

private:
    bool _enabled = false;
    int _sock_fd = -1;
    bool _auth = false;

    Kikan::Entity* _player{};
    std::map<uint16_t, Kikan::Entity*> _enemies;

    void addEnemy(uint16_t id);
};


#endif //LOTUS_NETWORKINGCLIENTSYSTEM_H
