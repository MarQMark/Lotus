#include "map/Map.h"
#include "components/BgSprite.h"
#include "util/ResourceManager.h"
#include "Kikan/ecs/components/AASprite.h"
#include "Kikan/ecs/components/LineQuadSprite.h"
#include "components/SColliderComponent.h"
#include "Kikan/ecs/components/QuadSprite.h"

std::map<std::string, Map*> MapManager::s_maps;

Map::Map() {
    // TODO: This should be an actual parser but it would take too long. Too bad!

   load_default_map();
}

Map::~Map() {
    if(_loaded_scene)
        unload();

    for(auto* e : _entities)
        delete e;
}

void Map::load(Kikan::Scene *scene) {
    if(_loaded_scene)
        return;

    for(auto* e : _entities)
        scene->addEntity(e);

    _loaded_scene = scene;
}

void Map::unload() {
    if(!_loaded_scene)
        return;

    for(auto* e : _entities)
        _loaded_scene->removeEntity(e);

    _loaded_scene = nullptr;
}

void Map::load_default_map() {
    // Clouds
    {
        auto* entity = new Kikan::Entity;
        auto* sprite = new Kikan::AASprite;
        sprite->offset = glm::vec2(0, 562.5f);
        sprite->dimensions = glm::vec2(3000, 562.5f);
        sprite->textureID = ResourceManager::get<TextureResource>(Resource::ID::TEX_OUTER_WALL_CLOUDS)->getID();
        sprite->layer = .2;
        sprite->color = glm::vec4(.5f);
        entity->addComponent(sprite);
        auto* bgSprite = new BgSprite();
        bgSprite->vel.x = -.01f;
        bgSprite->id = BgSprite::ID::CLOUDS;
        entity->addComponent(bgSprite);
        _entities.push_back(entity);
    }
    // Foreground
    {
        auto* entity = new Kikan::Entity;
        auto* sprite = new Kikan::AASprite;
        sprite->offset = glm::vec2(0, 562.5f);
        sprite->dimensions = glm::vec2(1000, 562.5f);
        sprite->textureID = ResourceManager::get<TextureResource>(Resource::ID::TEX_OUTER_WALL_BACKGROUND)->getID();
        sprite->layer = .1;
        sprite->color = glm::vec4(.5f);
        entity->addComponent(sprite);
        _entities.push_back(entity);
    }
    // Background
    {
        auto* entity = new Kikan::Entity;
        auto* sprite = new Kikan::AASprite;
        sprite->offset = glm::vec2(0, 562.5f);
        sprite->dimensions = glm::vec2(1000, 562.5f);
        sprite->textureID = ResourceManager::get<TextureResource>(Resource::ID::TEX_OUTER_WALL_FOREGROUND)->getID();
        sprite->layer = -.1;
        sprite->color = glm::vec4(.5f);
        entity->addComponent(sprite);
        _entities.push_back(entity);
    }


    // Ground
    {
        auto *entity = new Kikan::Entity();
        entity->getComponent<Kikan::Transform>()->position = glm::vec3(0, 81, 0);


        auto *collider = new SColliderComponent();
        collider->dimensions = glm::vec2(1000, 81);
        entity->addComponent(collider);

        auto *sprite = new Kikan::QuadSprite();
        glm::vec2 pos = entity->getComponent<Kikan::Transform>()->position;
        sprite->points[0] = pos + glm::vec2(0, 0);
        sprite->points[1] = pos + glm::vec2(collider->dimensions.x,0);
        sprite->points[2] = pos + glm::vec2(collider->dimensions.x, -collider->dimensions.y);
        sprite->points[3] = pos + glm::vec2(0, -collider->dimensions.y);
        sprite->color = glm::vec4(105./255., 97./255., 84./255., 1);
        sprite->layer = -0.2;
        entity->addComponent(sprite);

#ifdef DEBUG
        auto *hitbox = new Kikan::LineQuadSprite();
        hitbox->dimensions = collider->dimensions;
        hitbox->color = glm::vec4(.9, .5, .5, 1);
        hitbox->thickness = 2;
        hitbox->layer = -0.9;
        entity->addComponent(hitbox);
#endif

        _entities.push_back(entity);
    }
    // Left Wall
    {
        auto *entity = new Kikan::Entity();
        entity->getComponent<Kikan::Transform>()->position = glm::vec3(1000, 1000, 0);

        auto *collider = new SColliderComponent();
        collider->dimensions = glm::vec2(20, 1000);
        entity->addComponent(collider);


#ifdef DEBUG
        auto *sprite = new Kikan::LineQuadSprite();
        sprite->dimensions = collider->dimensions;
        sprite->color = glm::vec4(.4, .4, .4, 1);
        sprite->thickness = 5;
        entity->addComponent(sprite);
#endif
        _entities.push_back(entity);
    }
    // Right Wall
    {
        auto *entity = new Kikan::Entity();
        entity->getComponent<Kikan::Transform>()->position = glm::vec3(-20, 1000, 0);

        auto *collider = new SColliderComponent();
        collider->dimensions = glm::vec2(20, 1000);
        entity->addComponent(collider);

#ifdef DEBUG
        auto *sprite = new Kikan::LineQuadSprite();
        sprite->dimensions = collider->dimensions;
        sprite->color = glm::vec4(.4, .4, .4, 1);
        sprite->thickness = 5;
        entity->addComponent(sprite);
#endif

        _entities.push_back(entity);
    }
    // Sky box
    {
        auto *entity = new Kikan::Entity();
        entity->getComponent<Kikan::Transform>()->position = glm::vec3(-20, 1000, 0);

        auto *collider = new SColliderComponent();
        collider->dimensions = glm::vec2(1000, 100);
        entity->addComponent(collider);

#ifdef DEBUG
        auto *sprite = new Kikan::LineQuadSprite();
        sprite->dimensions = collider->dimensions;
        sprite->color = glm::vec4(.4, .4, .4, 1);
        sprite->thickness = 5;
        entity->addComponent(sprite);
#endif

        _entities.push_back(entity);
    }

    // Broken roof left
    {
        auto *entity = new Kikan::Entity();
        entity->getComponent<Kikan::Transform>()->position = glm::vec3(135, 298, 0);


        auto *collider = new SColliderComponent();
        collider->dimensions = glm::vec2(95, 12);
        collider->disableSide(1);
        collider->playerCanFall = true;
        entity->addComponent(collider);

#ifdef DEBUG
        auto *sprite = new Kikan::LineQuadSprite();
        sprite->dimensions = collider->dimensions;
        sprite->color = glm::vec4(.9, .5, .5, 1);
        sprite->thickness = 2;
        sprite->layer = -0.9;
        entity->addComponent(sprite);
#endif

        _entities.push_back(entity);
    }

    // Broken roof right
    {
        auto *entity = new Kikan::Entity();
        entity->getComponent<Kikan::Transform>()->position = glm::vec3(312, 298, 0);


        auto *collider = new SColliderComponent();
        collider->dimensions = glm::vec2(130, 12);
        collider->disableSide(1);
        collider->playerCanFall = true;
        entity->addComponent(collider);

#ifdef DEBUG
        auto *sprite = new Kikan::LineQuadSprite();
        sprite->dimensions = collider->dimensions;
        sprite->color = glm::vec4(.9, .5, .5, 1);
        sprite->thickness = 2;
        sprite->layer = -0.9;
        entity->addComponent(sprite);
#endif

        _entities.push_back(entity);
    }

    // beeeeeeeg roof
    {
        auto *entity = new Kikan::Entity();
        entity->getComponent<Kikan::Transform>()->position = glm::vec3(96, 192, 0);


        auto *collider = new SColliderComponent();
        collider->dimensions = glm::vec2(396, 12);
        collider->disableSide(1);
        collider->playerCanFall = true;
        entity->addComponent(collider);

#ifdef DEBUG
        auto *sprite = new Kikan::LineQuadSprite();
        sprite->dimensions = collider->dimensions;
        sprite->color = glm::vec4(.9, .5, .5, 1);
        sprite->thickness = 2;
        sprite->layer = -0.9;
        entity->addComponent(sprite);
#endif

        _entities.push_back(entity);
    }

    // train track
    {
        auto *entity = new Kikan::Entity();
        entity->getComponent<Kikan::Transform>()->position = glm::vec3(0, 412, 0);


        auto *collider = new SColliderComponent();
        collider->dimensions = glm::vec2(910, 12);
        collider->disableSide(1);
        collider->playerCanFall = true;
        entity->addComponent(collider);

#ifdef DEBUG
        auto *sprite = new Kikan::LineQuadSprite();
        sprite->dimensions = collider->dimensions;
        sprite->color = glm::vec4(.9, .5, .5, 1);
        sprite->thickness = 2;
        sprite->layer = -0.9;
        entity->addComponent(sprite);
#endif

        _entities.push_back(entity);
    }

    // roof right
    {
        auto *entity = new Kikan::Entity();
        entity->getComponent<Kikan::Transform>()->position = glm::vec3(858, 256, 0);


        auto *collider = new SColliderComponent();
        collider->dimensions = glm::vec2(142, 12);
        collider->disableSide(1);
        collider->playerCanFall = true;
        entity->addComponent(collider);

#ifdef DEBUG
        auto *sprite = new Kikan::LineQuadSprite();
        sprite->dimensions = collider->dimensions;
        sprite->color = glm::vec4(.9, .5, .5, 1);
        sprite->thickness = 2;
        sprite->layer = -0.9;
        entity->addComponent(sprite);
#endif

        _entities.push_back(entity);
    }

    // elevated floor
    {
        auto *entity = new Kikan::Entity();
        entity->getComponent<Kikan::Transform>()->position = glm::vec3(1000 -195, 143, 0);


        auto *collider = new SColliderComponent();
        collider->dimensions = glm::vec2(195, 62);
        entity->addComponent(collider);

#ifdef DEBUG
        auto *sprite = new Kikan::LineQuadSprite();
        sprite->dimensions = collider->dimensions;
        sprite->color = glm::vec4(.9, .5, .5, 1);
        sprite->thickness = 2;
        sprite->layer = -0.9;
        entity->addComponent(sprite);
#endif

        _entities.push_back(entity);
    }

    _spawnpoints.push_back(glm::vec2(50, 700));
    _spawnpoints.push_back(glm::vec2(800, 700));
    _spawnpoints.push_back(glm::vec2(50, 400));
    _spawnpoints.push_back(glm::vec2(800, 400));
}

void Map::spawnPlayers(std::vector<Kikan::Entity *> &players) {
    std::vector<Kikan::Entity *> entites(players.size());

    // TODO: sort by some metric
    for(int i = 0; i < players.size(); i ++)
        entites[i] = players[i];


    for(int i = 0; i < entites.size(); i ++){
        entites[i]->getComponent<Kikan::Transform>()->position = glm::vec3(_spawnpoints[i % _spawnpoints.size()], 0);
    }
}

