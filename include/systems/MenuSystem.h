#ifndef LOTUS_MENUSYSTEM_H
#define LOTUS_MENUSYSTEM_H

#include "Kikan/ecs/systems/ISystem.h"

class MenuSystem : public Kikan::ISystem {
public:
    MenuSystem();

    void update(double dt) override;

private:
    float _text_init_pos = -1;

    void update_main();
    void update_connect();
    void update_lobby();
};


#endif //LOTUS_MENUSYSTEM_H
