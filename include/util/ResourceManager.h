#ifndef LOTUS_RESOURCEMANAGER_H
#define LOTUS_RESOURCEMANAGER_H

#include <map>
#include <vector>
#include <string>
#include "Kikan/renderer/stdRenderer/buffers/Texture2D.h"

#define STB_IMAGE_STATIC
#include "stb_image/stb_image.h"
#include "glm/vec2.hpp"

class Resource{
public:
    enum ID{
        TEX_OUTER_WALL_BACKGROUND,
        TEX_OUTER_WALL_FOREGROUND,
        TEX_OUTER_WALL_CLOUDS,
        TEX_OUTER_WALL_TRAIN,

        SS_FIRE_PLAYER,
        SS_EARTH_PLAYER,
        SS_AIR_PLAYER,
        SS_WATER_PLAYER,

        SS_FIRE_ATTACK,
        SS_EARTH_ATTACK,
        SS_AIR_ATTACK,
        SS_WATER_ATTACK,

        SS_HEALTHBAR_ENEMY,
    };
};

class TextureResource : public Resource{
public:
    TextureResource(const std::string& path){
        int mapImgBPP;;
        unsigned char* buff = stbi_load(path.c_str(), &_width, &_height, &mapImgBPP, 4);
        _texture2D = new Kikan::Texture2D(_width, _height, buff);
    };

    GLuint getID(){
        return _texture2D->get();
    }
    int getWidth() const{
        return _width;
    }
    int getHeight() const{
        return _height;
    }

protected:
    Kikan::Texture2D* _texture2D;
    int _width = 0;
    int _height = 0;
};

class SpriteSheetResource : public TextureResource {
public:
    SpriteSheetResource(const std::string &path) : TextureResource(path) {}
    ~SpriteSheetResource(){
        for (auto* s : _sprites)
            delete s;
    }

    void addGrid(uint32_t w, uint32_t h) {
        uint32_t rows = _height / h;
        for (; rows > 0 ; rows--) {
            addRow(w, h);
        }
    }
    void addRow(uint32_t w, uint32_t h, uint32_t amount = 0, uint32_t offsetX = 0, uint32_t offsetY = 0) {
        if(amount == 0)
            amount = _width / w;

        _rows.push_back(amount);
        _last_row += offsetY;
        uint32_t x = offsetX;
        for (; amount > 0; amount--) {
            _sprites.push_back(new Sprite(x, _last_row, w, h, _width, _height));
            x += w;
        }
        _last_row += h;
    }
    void addSingle(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
        _sprites.push_back(new Sprite(x, y, w, h, _width, _height));
    }

    void getTexCoords(glm::vec2 coords[4], uint32_t id){
        auto* sprite = _sprites[id];
        coords[0] = sprite->tl;
        coords[1] = sprite->tr;
        coords[2] = sprite->br;
        coords[3] = sprite->bl;
    }

    void getTexCoords(glm::vec2 coords[4], uint32_t row, uint32_t column){
        uint32_t id = column;
        for(int i = 0; row > 0; i++, row--){
            id += _rows[i];
        }
        getTexCoords(coords, id);
    }

    uint32_t getSpriteID(uint32_t row, uint32_t column){
        uint32_t id = column;
        for(int i = 0; row > 0; i++, row--){
            id += _rows[i];
        }
        return id;
    }

    uint32_t getRowAmount(uint32_t row){
        return _rows[row];
    }

private:
    struct Sprite{
    public:
        Sprite(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t absW, uint32_t absH) : x(x), y(y), w(w), h(h) {
            float normX = (float)x / (float)absW;
            float normY = 1.f - (float)y / (float)absH;
            float normW = (float)w / (float)absW;
            float normH = (float)h / (float)absH;

            tl = glm::vec2(normX, normY);
            tr = glm::vec2(normX + normW, normY);
            br = glm::vec2(normX + normW, normY - normH);
            bl = glm::vec2(normX, normY - normH);
        }

        uint32_t x, y, w, h;
        glm::vec2 tl, tr, br, bl;
    };

    std::vector<Sprite*> _sprites;
    std::vector<uint32_t> _rows; // counts how many sprites are in each row

    uint32_t _last_row = 0;
};

class ResourceManager {
public:
    template<class T>
    static void add(Resource* resource, uint32_t id){
        if(!resource)
            return;

        s_textures[calc_id(id, getID<T>())] = resource;
    }

    template<class T>
    static T* get(uint32_t id) {
        return (T*)s_textures[calc_id(id, getID<T>())];
    }

private:
    template<class T>
    static uint32_t getID(){
        std::string sig = typeid(T).name();

        if (s_resource_ids.count(sig))
            return s_resource_ids[sig];

        s_resource_ids.insert(std::make_pair(sig, s_last_id++));
        return s_last_id - 1;
    }

    static uint64_t calc_id(uint32_t id, uint32_t type){
        return ((uint64_t)id << 32) | type;
    }

    static std::map<uint64_t, Resource*> s_textures;
    static std::map<std::string, uint32_t> s_resource_ids;
    static uint32_t s_last_id;
};


#endif //LOTUS_RESOURCEMANAGER_H
