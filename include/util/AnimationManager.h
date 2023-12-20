#ifndef LOTUS_ANIMATIONMANAGER_H
#define LOTUS_ANIMATIONMANAGER_H

#include <map>
#include <utility>
#include <vector>
#include <chrono>
#include "ResourceManager.h"
#include "Kikan/core/Logging.h"

class Animation {
public:
    enum ID{
        FIRE_IDLE_RIGHT,
        FIRE_IDLE_LEFT,
        FIRE_ATTACK_RIGHT,
        FIRE_ATTACK_LEFT,
        FIRE_JUMP_RIGHT,
        FIRE_JUMP_LEFT,
        FIRE_MOV_RIGHT,
        FIRE_MOV_LEFT,

    };

    Animation(SpriteSheetResource* spriteSheet, std::vector<uint32_t> frames, double speed) : _sprite_sheet(spriteSheet), _frames(std::move(frames)), _speed(speed) {
        _last_time = std::chrono::high_resolution_clock::now();
    }
    Animation(SpriteSheetResource* spriteSheet, uint32_t row, double speed) : _sprite_sheet(spriteSheet), _speed(speed){
        uint32_t amount = spriteSheet->getRowAmount(row);
        uint32_t startID = spriteSheet->getSpriteID(row, 0);
        _frames.resize(amount);
        for(uint32_t i = 0; i < amount; i++)
            _frames[i] = startID + i;

        _last_time = std::chrono::high_resolution_clock::now();
    }

    void getFrame(glm::vec2 texCoords[4]){
        double dt = ((std::chrono::duration<double, std::milli>)(std::chrono::high_resolution_clock::now() - _last_time)).count();
        _com_time += dt;

        _curr_frame = ((uint32_t)(_com_time / _speed) + _curr_frame) % _frames.size();
        _sprite_sheet->getTexCoords(texCoords, _frames[_curr_frame]);

        if(_com_time > _speed){
            _com_time -= _speed;
        }

        _last_time = std::chrono::high_resolution_clock::now();
    }
    GLuint getID(){
        return _sprite_sheet->getID();
    }
    void setSpeed(double speed){
        _speed = speed;
    }
    void pause(){
        paused = true;
    }
    void resume(){
        paused = false;
    }
    void reset(){
        _curr_frame = 0;
        _com_time = 0;
        _last_time = std::chrono::high_resolution_clock::now();
    }
private:
    SpriteSheetResource* _sprite_sheet;
    std::vector<uint32_t> _frames;
    uint32_t _curr_frame = 0;

    std::chrono::high_resolution_clock::time_point _last_time;

    double _speed;
    double _com_time = 0;
    bool paused = false;
};


class AnimationManager {
public:
    static Animation* getAnimation(uint32_t id){
        return s_animations[id];
    }

    static void addAnimation(Animation* animation, uint32_t id){
        s_animations[id] = animation;
    }

private:
    static std::map<uint32_t, Animation*> s_animations;
};


#endif //LOTUS_ANIMATIONMANAGER_H
