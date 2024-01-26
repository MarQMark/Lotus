#ifndef LOTUS_MENUSYSTEM_H
#define LOTUS_MENUSYSTEM_H

#include "Kikan/ecs/systems/ISystem.h"

class MenuSystem : public Kikan::ISystem {
public:
    MenuSystem();

    void update(double dt) override;

private:
    float _text_init_pos = -1;

    Kikan::Entity* _victor{};
    double _victory_duration = 5000;

    void update_main();
    void update_connect();
    void update_lobby();
    void update_victory(double dt);
};


#endif //LOTUS_MENUSYSTEM_H
