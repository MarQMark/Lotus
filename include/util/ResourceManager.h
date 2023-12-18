#ifndef LOTUS_RESOURCEMANAGER_H
#define LOTUS_RESOURCEMANAGER_H

#include <map>
#include <string>
#include "Kikan/renderer/stdRenderer/buffers/Texture2D.h"

#define STB_IMAGE_STATIC
#include "stb_image/stb_image.h"

class Resource{
public:
    enum ID{
        OUTER_WALL_BACKGROUND,
        OUTER_WALL_FOREGROUND,
        OUTER_WALL_CLOUDS,
        OUTER_WALL_TRAIN,
    };
};

class TextureResource : public Resource{
public:
    TextureResource(const std::string& path){
        int mapImgBPP;;
        unsigned char* buff = stbi_load(path.c_str(), &_width, &_height, &mapImgBPP, 4);
        _texture2D = new Kikan::Renderer::Texture2D(_width, _height, buff);
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

private:
    Kikan::Renderer::Texture2D* _texture2D;
    int _width = 0;
    int _height = 0;
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
