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
        // FIRE
        FIRE_PLAYER_IDLE_R,
        FIRE_PLAYER_IDLE_L,
        FIRE_PLAYER_ATTACK_R,
        FIRE_PLAYER_ATTACK_L,
        FIRE_PLAYER_JUMP_R,
        FIRE_PLAYER_JUMP_L,
        FIRE_PLAYER_MOV_R,
        FIRE_PLAYER_MOV_L,
        FIRE_PLAYER_JUMP_ABL_R,
        FIRE_PLAYER_JUMP_ABL_L,

        FIRE_ATTACK_R,
        FIRE_ATTACK_L,

        // EARTH
        EARTH_PLAYER_IDLE_R,
        EARTH_PLAYER_IDLE_L,
        EARTH_PLAYER_ATTACK_R,
        EARTH_PLAYER_ATTACK_L,
        EARTH_PLAYER_JUMP_R,
        EARTH_PLAYER_JUMP_L,
        EARTH_PLAYER_MOV_R,
        EARTH_PLAYER_MOV_L,

        EARTH_PLAYER_ULT,

        EARTH_ATTACK_R,
        EARTH_ATTACK_L,

        // AIR
        AIR_PLAYER_IDLE_R,
        AIR_PLAYER_IDLE_L,
        AIR_PLAYER_ATTACK_R,
        AIR_PLAYER_ATTACK_L,
        AIR_PLAYER_JUMP_R,
        AIR_PLAYER_JUMP_L,
        AIR_PLAYER_MOV_R,
        AIR_PLAYER_MOV_L,

        AIR_ATTACK_R,
        AIR_ATTACK_L,

        // WATER
        WATER_PLAYER_IDLE_R,
        WATER_PLAYER_IDLE_L,
        WATER_PLAYER_ATTACK_R,
        WATER_PLAYER_ATTACK_L,
        WATER_PLAYER_JUMP_R,
        WATER_PLAYER_JUMP_L,
        WATER_PLAYER_MOV_R,
        WATER_PLAYER_MOV_L,

        WATER_ATTACK_R,
        WATER_ATTACK_L,
    };

    Animation(SpriteSheetResource* spriteSheet, std::vector<uint32_t> frames, double speed) : _sprite_sheet(spriteSheet), _frames(std::move(frames)), _speed(speed) {
    }
    Animation(SpriteSheetResource* spriteSheet, uint32_t row, double speed) : _sprite_sheet(spriteSheet), _speed(speed){
        uint32_t amount = spriteSheet->getRowAmount(row);
        uint32_t startID = spriteSheet->getSpriteID(row, 0);
        _frames.resize(amount);
        for(uint32_t i = 0; i < amount; i++)
            _frames[i] = startID + i;
    }

    void getFrame(glm::vec2 texCoords[4], double &currentFrame, std::chrono::high_resolution_clock::time_point &lastTime) {
        double dt = ((std::chrono::duration<double, std::milli>)(std::chrono::high_resolution_clock::now() - lastTime)).count();

        currentFrame = ((dt / _speed) + currentFrame);
        if(currentFrame > (double)_frames.size())
            currentFrame = 0;

        _sprite_sheet->getTexCoords(texCoords, _frames[(uint32_t)currentFrame]);

        lastTime = std::chrono::high_resolution_clock::now();
    }
    GLuint getID(){
        return _sprite_sheet->getID();
    }
    void setSpeed(double speed){
        _speed = speed;
    }
    Animation::ID id;

private:
    SpriteSheetResource* _sprite_sheet;
    std::vector<uint32_t> _frames;
    double _speed;
};


class AnimationManager {
public:
    static Animation* getAnimation(uint32_t id){
        return s_animations[id];
    }

    static void addAnimation(Animation* animation, uint32_t id){
        s_animations[id] = animation;
        animation->id = (Animation::ID)id;
    }

private:
    static std::map<uint32_t, Animation*> s_animations;
};


#endif //LOTUS_ANIMATIONMANAGER_H
