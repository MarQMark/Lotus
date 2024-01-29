#ifndef LOTUS_MAP_H
#define LOTUS_MAP_H

#include <string>
#include <map>
#include <utility>

#include "Kikan/ecs/Entity.h"
#include "Kikan/ecs/Scene.h"

class Map {
public:
    Map();
    ~Map();

    std::string name;

    void load(Kikan::Scene* scene);
    void unload();

    void spawnPlayers(std::vector<Kikan::Entity*>& players);

private:
    void load_default_map();

    Kikan::Scene* _loaded_scene = nullptr;
    std::vector<Kikan::Entity*> _entities;
    std::vector<glm::vec2> _spawnpoints;
};

class MapManager{
public:
    static void add(std::string name, Map* map){
        map->name = std::move(name);
        s_maps[map->name] = map;
    }

    static Map* get(const std::string& name){
        if(s_maps.count(name))
            return s_maps[name];

        return nullptr;
    }

private:
    static std::map<std::string, Map*> s_maps;

};

#endif //LOTUS_MAP_H
