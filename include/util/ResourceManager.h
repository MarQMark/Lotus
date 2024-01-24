#ifndef LOTUS_RESOURCEMANAGER_H
#define LOTUS_RESOURCEMANAGER_H

#include <map>
#include <vector>
#include <string>
#include <typeinfo>
#include "Kikan/renderer/stdRenderer/buffers/Texture2D.h"

#define STB_IMAGE_STATIC
#include "stb_image/stb_image.h"
#include "glm/vec2.hpp"

class Resource{
public:
    enum ID{
        TEX_NULL,

        TEX_MARK_INDUSTRIES,
        TEX_MARK_INDUSTRIES_I,

        TEX_OUTER_WALL_BACKGROUND,
        TEX_OUTER_WALL_FOREGROUND,
        TEX_OUTER_WALL_CLOUDS,
        TEX_OUTER_WALL_TRAIN,

        TEX_EARTH_ABILITY,


        SS_NULL,

        SS_FIRE_PLAYER,
        SS_EARTH_PLAYER,
        SS_AIR_PLAYER,
        SS_WATER_PLAYER,

        SS_FIRE_ATTACK,
        SS_EARTH_ATTACK,
        SS_AIR_ATTACK,
        SS_WATER_ATTACK,

        SS_HEALTHBAR_ENEMY,
        SS_HEALTHBAR_FIRE,
        SS_HEALTHBAR_EARTH,
        SS_HEALTHBAR_AIR,
        SS_HEALTHBAR_WATER,

        SS_UI_COOLDOWN_FIRE,
        SS_UI_COOLDOWN_EARTH,
        SS_UI_COOLDOWN_AIR,
        SS_UI_COOLDOWN_WATER,
    };

    ID id;

    static void init(){
        if(_null_tex)
            return;

#define SQUARES 16
#define SQ_RES 16
        uint32_t size = SQUARES * SQ_RES * SQUARES * SQ_RES* 4;
        unsigned char buff[size];
        for(uint32_t i = 0; i < size; i += 4){
            char color = 0;
            int row = (i / 4) / (SQUARES * SQ_RES);
            if(row % (SQUARES * 2) < SQUARES){
                if((i / 4) % (SQUARES * 2) < SQUARES)
                    color = 255;
            }
            else{
                if((i / 4) % (SQUARES * 2) >= SQUARES)
                    color = 255;
            }


            buff[i + 0] = color;
            buff[i + 1] = 0;
            buff[i + 2] = color;
            buff[i + 3] = 255;
        }
        GLsizei width = SQUARES * SQ_RES;
        GLsizei height = SQUARES * SQ_RES;
#undef SQUARES
#undef SQ_RES

        struct Kikan::Texture2D::Options ops;
        ops.internalformat = GL_COMPRESSED_RGBA;
        _null_tex = new Kikan::Texture2D(width, height, buff, &ops);
    }

protected:
    static Kikan::Texture2D* _null_tex;
};

class TextureResource : public Resource{
public:
    TextureResource(const std::string& path){
        int mapImgBPP;;
        unsigned char* buff = stbi_load(path.c_str(), &_width, &_height, &mapImgBPP, 4);
        struct Kikan::Texture2D::Options ops;
        //ops.internalformat = GL_COMPRESSED_RGBA;
        _texture2D = new Kikan::Texture2D(_width, _height, buff, &ops);
        // This might cause a crash. Let's hope it doesn't, because else I would be even more confused
        free(buff);
    };
    TextureResource(unsigned char* buff, int width, int height) : _width(width), _height(height) {
        struct Kikan::Texture2D::Options ops;
        //ops.internalformat = GL_COMPRESSED_RGBA;
        _texture2D = new Kikan::Texture2D(width, height, buff, &ops);
    }

    TextureResource(){
        _is_empty = true;
    }

    void create(unsigned char* buff, int width, int height){
        struct Kikan::Texture2D::Options ops;
        //ops.internalformat = GL_COMPRESSED_RGBA;
        _texture2D = new Kikan::Texture2D(width, height, buff, &ops);
        _width = width;
        _height = height;

        _is_empty = false;
    }

    GLuint getID(){
        if(!_texture2D)
            return _null_tex->get();

        return _texture2D->get();
    }
    int getWidth() const{
        if(!_texture2D)
            return _null_tex->getWidth();

        return _width;
    }
    int getHeight() const{
        if(!_texture2D)
            return _null_tex->getHeight();

        return _height;
    }
    Kikan::Texture2D* getTexture2D(){
        if(!_texture2D)
            return _null_tex;

        return _texture2D;
    }
protected:
    bool _is_empty = false;

private:
    Kikan::Texture2D* _texture2D = nullptr;
    int _width = 0;
    int _height = 0;
};

class SpriteSheetResource : public TextureResource {
public:
    SpriteSheetResource(const std::string &path) : TextureResource(path) {}
    SpriteSheetResource(unsigned char* buff, int width, int height) : TextureResource(buff, width, height) {}
    SpriteSheetResource() : TextureResource() {
    }
    ~SpriteSheetResource(){
        for (auto* s : _sprites)
            delete s;
    }

    void addGrid(uint32_t w, uint32_t h) {
        uint32_t rows = getHeight() / h;
        for (; rows > 0 ; rows--) {
            addRow(w, h);
        }
    }
    void addRow(uint32_t w, uint32_t h, uint32_t amount = 0, uint32_t offsetX = 0, uint32_t offsetY = 0) {
        if(amount == 0)
            amount = getWidth() / w;

        _rows.push_back(amount);
        _last_row += offsetY;
        uint32_t x = offsetX;
        for (; amount > 0; amount--) {
            _sprites.push_back(new Sprite(x, _last_row, w, h, getWidth(), getHeight()));
            x += w;
        }
        _last_row += h;
    }
    void addSingle(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
        _sprites.push_back(new Sprite(x, y, w, h, getWidth(), getHeight()));
    }

    void getTexCoords(glm::vec2 coords[4], uint32_t id){
        if(_is_empty){
            coords[0] = glm::vec2(0, 1);
            coords[1] = glm::vec2(1, 1);
            coords[2] = glm::vec2(1, 0);
            coords[3] = glm::vec2(0, 0);
            return;
        }

        id = std::min(id, (uint32_t)_sprites.size() - 1);

        auto* sprite = _sprites[id];
        coords[0] = sprite->tl;
        coords[1] = sprite->tr;
        coords[2] = sprite->br;
        coords[3] = sprite->bl;
    }

    void getTexCoords(glm::vec2 coords[4], uint32_t row, uint32_t column){
        if(_is_empty){
            coords[0] = glm::vec2(0, 1);
            coords[1] = glm::vec2(1, 1);
            coords[2] = glm::vec2(1, 0);
            coords[3] = glm::vec2(0, 0);
            return;
        }

        uint32_t id = column;
        id = std::min(id, (uint32_t)_rows.size() - 1);

        for(int i = 0; row > 0; i++, row--){
            id += _rows[i];
        }
        getTexCoords(coords, id);
    }

    uint32_t getSpriteID(uint32_t row, uint32_t column){
        if(_is_empty)
            return 0;

        uint32_t id = column;
        id = std::min(id, (uint32_t)_rows.size() - 1);

        for(int i = 0; row > 0; i++, row--){
            id += _rows[i];
        }
        return id;
    }

    uint32_t getRowAmount(uint32_t row){
        if(_is_empty)
            return 1;

        row = std::min(row, (uint32_t)_rows.size() - 1);
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

        resource->id = (Resource::ID)id;
        s_textures[calc_id(id, getID<T>())] = resource;
    }

    template<class T>
    static T* get(uint32_t id) {
        if(s_textures.count(calc_id(id, getID<T>())))
            return (T*)s_textures[calc_id(id, getID<T>())];

        if(id < Resource::ID::SS_NULL){
            if(!s_textures.count(calc_id(Resource::ID::TEX_NULL, getID<T>())))
                add<TextureResource>(new TextureResource(), Resource::ID::TEX_NULL);
            add<T>(s_textures[calc_id(Resource::ID::TEX_NULL, getID<T>())], id);
            return (T*)s_textures[calc_id(Resource::ID::TEX_NULL, getID<T>())];
        }
        else{
            if(!s_textures.count(calc_id(Resource::ID::SS_NULL, getID<T>()))){
                auto* res = new SpriteSheetResource();
                res->addGrid(res->getWidth(), res->getHeight());
                add<SpriteSheetResource>(res, Resource::ID::SS_NULL);
            }
            add<T>(s_textures[calc_id(Resource::ID::TEX_NULL, getID<T>())], id);
            return (T*)s_textures[calc_id(Resource::ID::SS_NULL, getID<T>())];
        }
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
