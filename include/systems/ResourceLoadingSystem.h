#ifndef LOTUS_RESOURCELOADINGSYSTEM_H
#define LOTUS_RESOURCELOADINGSYSTEM_H

#include "Kikan/ecs/systems/ISystem.h"
#include <thread>

#define MAX_LOAD_CNT 15
#define MAX_LOAD_DELAY 100

class ResourceLoadingSystem : public Kikan::ISystem {
public:
    ResourceLoadingSystem();

    void update(double dt) override;
private:
    void loadResources();
    void create_sprite_sheets();

    struct LoadingData {
        unsigned char* buff;
        int width;
        int height;
    };

    std::map<int, LoadingData> _data;

    bool _init = true;
    bool _finished_loading = false;

    int _load_cnt = 0;
    double _load_delay = 0;

    std::thread* t;
};


#endif //LOTUS_RESOURCELOADINGSYSTEM_H
