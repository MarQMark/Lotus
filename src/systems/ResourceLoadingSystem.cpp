#include "systems/ResourceLoadingSystem.h"
#include "util/ResourceManager.h"
#include "util/AnimationManager.h"
#include "Constants.h"
#include "Kikan/core/Timer.h"

void loadTextures(){
    Kikan::Timer t("Textures");

    ResourceManager::add<TextureResource>(new TextureResource("res/Maps/OuterWall2/background.png"), Resource::ID::TEX_OUTER_WALL_BACKGROUND);
    ResourceManager::add<TextureResource>(new TextureResource("res/Maps/OuterWall2/foreground.png"), Resource::ID::TEX_OUTER_WALL_FOREGROUND);
    ResourceManager::add<TextureResource>(new TextureResource("res/Maps/OuterWall2/Clouds.png"),     Resource::ID::TEX_OUTER_WALL_CLOUDS);
    ResourceManager::add<TextureResource>(new TextureResource("res/Maps/OuterWall2/Train.png"),      Resource::ID::TEX_OUTER_WALL_TRAIN);

    ResourceManager::add<TextureResource>(new TextureResource("res/Earth/LaunchSprite.png"),      Resource::ID::TEX_EARTH_ABILITY);
}

ResourceLoadingSystem::ResourceLoadingSystem() {
    stbi_set_flip_vertically_on_load(1);
    loadTextures();
}


void ResourceLoadingSystem::loadResources() {

    int mapImgBPP, width, height;
    unsigned char* buff;

    buff = stbi_load("res/Fire/FirePlayerSpriteSheet.png", &width, &height, &mapImgBPP, 4);
    _data[Resource::ID::SS_FIRE_PLAYER] = LoadingData {
        .buff = buff,
        .width = width,
        .height = height,
    };


    buff = stbi_load("res/Earth/EarthPlayerSpriteSheet.png", &width, &height, &mapImgBPP, 4);
    _data[Resource::ID::SS_EARTH_PLAYER] = LoadingData {
        .buff = buff,
        .width = width,
        .height = height,
    };
    buff = stbi_load("res/Air/AirPlayerSpriteSheet.png", &width, &height, &mapImgBPP, 4);
    _data[Resource::ID::SS_AIR_PLAYER] = LoadingData {
        .buff = buff,
        .width = width,
        .height = height,
    };
    buff = stbi_load("res/Fire/FirePlayerSpriteSheet.png", &width, &height, &mapImgBPP, 4);
    _data[Resource::ID::SS_WATER_PLAYER] = LoadingData {
            .buff = buff,
            .width = width,
            .height = height,
    };
    // Attacks
    buff = stbi_load("res/Fire/fireballSprite.png", &width, &height, &mapImgBPP, 4);
    _data[Resource::ID::SS_FIRE_ATTACK] = LoadingData {
        .buff = buff,
        .width = width,
        .height = height,
    };
    buff = stbi_load("res/Earth/RockSprite.png", &width, &height, &mapImgBPP, 4);
    _data[Resource::ID::SS_EARTH_ATTACK] = LoadingData {
        .buff = buff,
        .width = width,
        .height = height,
    };
    buff = stbi_load("res/Air/AirAttack.png", &width, &height, &mapImgBPP, 4);
    _data[Resource::ID::SS_AIR_ATTACK] = LoadingData {
            .buff = buff,
            .width = width,
            .height = height,
    };

    // Healthbar
    buff = stbi_load("res/Assets/SpriteSheetHealthbarEnemy.png", &width, &height, &mapImgBPP, 4);
    _data[Resource::ID::SS_HEALTHBAR_ENEMY] = LoadingData {
            .buff = buff,
            .width = width,
            .height = height,
    };

    buff = stbi_load("res/Fire/SpriteSheetFireHealthbar.png", &width, &height, &mapImgBPP, 4);
    _data[Resource::ID::SS_HEALTHBAR_FIRE] = LoadingData {
        .buff = buff,
        .width = width,
        .height = height,
    };
    buff = stbi_load("res/Earth/SpriteSheetEarthHealthbar.png", &width, &height, &mapImgBPP, 4);
    _data[Resource::ID::SS_HEALTHBAR_EARTH] = LoadingData {
        .buff = buff,
        .width = width,
        .height = height,
    };
    buff = stbi_load("res/Air/SpriteSheetAirHealthbar.png", &width, &height, &mapImgBPP, 4);
    _data[Resource::ID::SS_HEALTHBAR_AIR] = LoadingData {
        .buff = buff,
        .width = width,
        .height = height,
    };
    buff = stbi_load("res/Water/SpriteSheetWaterHealthbar.png", &width, &height, &mapImgBPP, 4);
    _data[Resource::ID::SS_HEALTHBAR_WATER] = LoadingData {
            .buff = buff,
            .width = width,
            .height = height,
    };

    // UI cooldowns
    buff = stbi_load("res/Fire/SpriteSheetFireAbilities.png", &width, &height, &mapImgBPP, 4);
    _data[Resource::ID::SS_UI_COOLDOWN_FIRE] = LoadingData {
        .buff = buff,
        .width = width,
        .height = height,
    };
    buff = stbi_load("res/Earth/SpriteSheetEarthAbilities.png", &width, &height, &mapImgBPP, 4);
    _data[Resource::ID::SS_UI_COOLDOWN_EARTH] = LoadingData {
        .buff = buff,
        .width = width,
        .height = height,
    };
    buff = stbi_load("res/Air/SpriteSheetAirAbilities.png", &width, &height, &mapImgBPP, 4);
    _data[Resource::ID::SS_UI_COOLDOWN_AIR] = LoadingData {
        .buff = buff,
        .width = width,
        .height = height,
    };
    buff = stbi_load("res/Fire/SpriteSheetFireAbilities.png", &width, &height, &mapImgBPP, 4);
    _data[Resource::ID::SS_UI_COOLDOWN_WATER] = LoadingData {
            .buff = buff,
            .width = width,
            .height = height,
    };

    printf("Thread end\n");
    _finished_loading = true;
}

void ResourceLoadingSystem::create_sprite_sheets() {
    SpriteSheetResource* spriteSheet;

    switch (_load_cnt) {
        // Players
        case 0:
            spriteSheet = new SpriteSheetResource(_data[Resource::ID::SS_FIRE_PLAYER].buff, _data[Resource::ID::SS_FIRE_PLAYER].width, _data[Resource::ID::SS_FIRE_PLAYER].height);
            spriteSheet->addGrid(650, 1200);
            ResourceManager::add<SpriteSheetResource>(spriteSheet, Resource::ID::SS_FIRE_PLAYER);
            break;
        case 1:
            spriteSheet = new SpriteSheetResource(_data[Resource::ID::SS_EARTH_PLAYER].buff, _data[Resource::ID::SS_EARTH_PLAYER].width, _data[Resource::ID::SS_EARTH_PLAYER].height);
            spriteSheet->addGrid(650, 1200);
            ResourceManager::add<SpriteSheetResource>(spriteSheet, Resource::ID::SS_EARTH_PLAYER);
            break;
        case 2:
            spriteSheet = new SpriteSheetResource(_data[Resource::ID::SS_AIR_PLAYER].buff, _data[Resource::ID::SS_AIR_PLAYER].width, _data[Resource::ID::SS_AIR_PLAYER].height);
            spriteSheet->addGrid(650, 1200);
            ResourceManager::add<SpriteSheetResource>(spriteSheet, Resource::ID::SS_AIR_PLAYER);
            break;
        case 3:
            // TODO: Change to actual water player once finished
            spriteSheet = new SpriteSheetResource(_data[Resource::ID::SS_WATER_PLAYER].buff, _data[Resource::ID::SS_WATER_PLAYER].width, _data[Resource::ID::SS_WATER_PLAYER].height);
            spriteSheet->addGrid(650, 1200);
            ResourceManager::add<SpriteSheetResource>(spriteSheet, Resource::ID::SS_WATER_PLAYER);
            break;

        // Attacks
        case 4:
            spriteSheet = new SpriteSheetResource(_data[Resource::ID::SS_FIRE_ATTACK].buff, _data[Resource::ID::SS_FIRE_ATTACK].width, _data[Resource::ID::SS_FIRE_ATTACK].height);
            spriteSheet->addGrid(300, 150);
            ResourceManager::add<SpriteSheetResource>(spriteSheet, Resource::ID::SS_FIRE_ATTACK);
            break;
        case 5:
            spriteSheet = new SpriteSheetResource(_data[Resource::ID::SS_EARTH_ATTACK].buff, _data[Resource::ID::SS_EARTH_ATTACK].width, _data[Resource::ID::SS_EARTH_ATTACK].height);
            spriteSheet->addGrid(340, 180);
            ResourceManager::add<SpriteSheetResource>(spriteSheet, Resource::ID::SS_EARTH_ATTACK);
            break;
        case 6:
            spriteSheet = new SpriteSheetResource(_data[Resource::ID::SS_AIR_ATTACK].buff, _data[Resource::ID::SS_AIR_ATTACK].width, _data[Resource::ID::SS_AIR_ATTACK].height);
            spriteSheet->addGrid(195, 485);
            ResourceManager::add<SpriteSheetResource>(spriteSheet, Resource::ID::SS_AIR_ATTACK);
            break;

        // Healthbar
        case 7:
            spriteSheet = new SpriteSheetResource(_data[Resource::ID::SS_HEALTHBAR_ENEMY].buff, _data[Resource::ID::SS_HEALTHBAR_ENEMY].width, _data[Resource::ID::SS_HEALTHBAR_ENEMY].height);
            spriteSheet->addGrid(400, 50);
            ResourceManager::add<SpriteSheetResource>(spriteSheet, Resource::ID::SS_HEALTHBAR_ENEMY);
            break;
        case 8:
            spriteSheet = new SpriteSheetResource(_data[Resource::ID::SS_HEALTHBAR_FIRE].buff, _data[Resource::ID::SS_HEALTHBAR_FIRE].width, _data[Resource::ID::SS_HEALTHBAR_FIRE].height);
            spriteSheet->addGrid(5780, 960);
            ResourceManager::add<SpriteSheetResource>(spriteSheet, Resource::ID::SS_HEALTHBAR_FIRE);
            break;
        case 9:
            spriteSheet = new SpriteSheetResource(_data[Resource::ID::SS_HEALTHBAR_EARTH].buff, _data[Resource::ID::SS_HEALTHBAR_EARTH].width, _data[Resource::ID::SS_HEALTHBAR_EARTH].height);
            spriteSheet->addGrid(5780, 960);
            ResourceManager::add<SpriteSheetResource>(spriteSheet, Resource::ID::SS_HEALTHBAR_EARTH);
            break;
        case 10:
            spriteSheet = new SpriteSheetResource(_data[Resource::ID::SS_HEALTHBAR_AIR].buff, _data[Resource::ID::SS_HEALTHBAR_AIR].width, _data[Resource::ID::SS_HEALTHBAR_AIR].height);
            spriteSheet->addGrid(5780, 960);
            ResourceManager::add<SpriteSheetResource>(spriteSheet, Resource::ID::SS_HEALTHBAR_AIR);
            break;
        case 11:
            spriteSheet = new SpriteSheetResource(_data[Resource::ID::SS_HEALTHBAR_WATER].buff, _data[Resource::ID::SS_HEALTHBAR_WATER].width, _data[Resource::ID::SS_HEALTHBAR_WATER].height);
            spriteSheet->addGrid(5780, 960);
            ResourceManager::add<SpriteSheetResource>(spriteSheet, Resource::ID::SS_HEALTHBAR_WATER);
            break;

        // UI cooldowns
        case 12:
            spriteSheet = new SpriteSheetResource(_data[Resource::ID::SS_UI_COOLDOWN_FIRE].buff, _data[Resource::ID::SS_UI_COOLDOWN_FIRE].width, _data[Resource::ID::SS_UI_COOLDOWN_FIRE].height);
            spriteSheet->addGrid(910, 910);
            ResourceManager::add<SpriteSheetResource>(spriteSheet, Resource::ID::SS_UI_COOLDOWN_FIRE);
            break;
        case 13:
            spriteSheet = new SpriteSheetResource(_data[Resource::ID::SS_UI_COOLDOWN_EARTH].buff, _data[Resource::ID::SS_UI_COOLDOWN_EARTH].width, _data[Resource::ID::SS_UI_COOLDOWN_EARTH].height);
            spriteSheet->addGrid(910, 910);
            ResourceManager::add<SpriteSheetResource>(spriteSheet, Resource::ID::SS_UI_COOLDOWN_EARTH);
            break;
        case 14:
            spriteSheet = new SpriteSheetResource(_data[Resource::ID::SS_UI_COOLDOWN_AIR].buff, _data[Resource::ID::SS_UI_COOLDOWN_AIR].width, _data[Resource::ID::SS_UI_COOLDOWN_AIR].height);
            spriteSheet->addGrid(910, 910);
            ResourceManager::add<SpriteSheetResource>(spriteSheet, Resource::ID::SS_UI_COOLDOWN_AIR);
            break;
        case MAX_LOAD_CNT:
            spriteSheet = new SpriteSheetResource(_data[Resource::ID::SS_UI_COOLDOWN_WATER].buff, _data[Resource::ID::SS_UI_COOLDOWN_WATER].width, _data[Resource::ID::SS_UI_COOLDOWN_WATER].height);
            spriteSheet->addGrid(910, 910);
            ResourceManager::add<SpriteSheetResource>(spriteSheet, Resource::ID::SS_UI_COOLDOWN_WATER);
            break;

    }

    _load_cnt++;
}

void createAnimations(){
    Kikan::Timer t("Animations");
    Animation* animation;

    // FIRE PLAYER
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_FIRE_PLAYER), std::vector<uint32_t>{0}, 0);
    AnimationManager::addAnimation(animation, Animation::ID::FIRE_PLAYER_IDLE_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_FIRE_PLAYER), std::vector<uint32_t>{1}, 0);
    AnimationManager::addAnimation(animation, Animation::ID::FIRE_PLAYER_IDLE_L);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_FIRE_PLAYER), std::vector<uint32_t>{4}, 0);
    AnimationManager::addAnimation(animation, Animation::ID::FIRE_PLAYER_ATTACK_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_FIRE_PLAYER), std::vector<uint32_t>{5}, 0);
    AnimationManager::addAnimation(animation, Animation::ID::FIRE_PLAYER_ATTACK_L);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_FIRE_PLAYER), std::vector<uint32_t>{8}, 0);
    AnimationManager::addAnimation(animation, Animation::ID::FIRE_PLAYER_JUMP_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_FIRE_PLAYER), std::vector<uint32_t>{9}, 0);
    AnimationManager::addAnimation(animation, Animation::ID::FIRE_PLAYER_JUMP_L);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_FIRE_PLAYER), 3, MOVE_ANI_SPEED[Nation::FIRE]);
    AnimationManager::addAnimation(animation, Animation::ID::FIRE_PLAYER_MOV_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_FIRE_PLAYER), 4, MOVE_ANI_SPEED[Nation::FIRE]);
    AnimationManager::addAnimation(animation, Animation::ID::FIRE_PLAYER_MOV_L);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_FIRE_PLAYER), 5, MOVE_ANI_SPEED[Nation::FIRE]);
    AnimationManager::addAnimation(animation, Animation::ID::FIRE_PLAYER_JUMP_ABL_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_FIRE_PLAYER), 6, MOVE_ANI_SPEED[Nation::FIRE]);
    AnimationManager::addAnimation(animation, Animation::ID::FIRE_PLAYER_JUMP_ABL_L);

    // EARTH PLAYER
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_EARTH_PLAYER), std::vector<uint32_t>{0}, 0);
    AnimationManager::addAnimation(animation, Animation::ID::EARTH_PLAYER_IDLE_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_EARTH_PLAYER), std::vector<uint32_t>{1}, 0);
    AnimationManager::addAnimation(animation, Animation::ID::EARTH_PLAYER_IDLE_L);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_EARTH_PLAYER), 1, ATK_ANI_SPEED[Nation::EARTH]);
    AnimationManager::addAnimation(animation, Animation::ID::EARTH_PLAYER_ATTACK_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_EARTH_PLAYER), 2, ATK_ANI_SPEED[Nation::EARTH]);
    AnimationManager::addAnimation(animation, Animation::ID::EARTH_PLAYER_ATTACK_L);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_EARTH_PLAYER), std::vector<uint32_t>{12}, 0);
    AnimationManager::addAnimation(animation, Animation::ID::EARTH_PLAYER_JUMP_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_EARTH_PLAYER), std::vector<uint32_t>{13}, 0);
    AnimationManager::addAnimation(animation, Animation::ID::EARTH_PLAYER_JUMP_L);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_EARTH_PLAYER), 4, MOVE_ANI_SPEED[Nation::EARTH]);
    AnimationManager::addAnimation(animation, Animation::ID::EARTH_PLAYER_MOV_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_EARTH_PLAYER), 5, MOVE_ANI_SPEED[Nation::EARTH]);
    AnimationManager::addAnimation(animation, Animation::ID::EARTH_PLAYER_MOV_L);

    // AIR PLAYER
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_AIR_PLAYER), std::vector<uint32_t>{0}, 0);
    AnimationManager::addAnimation(animation, Animation::ID::AIR_PLAYER_IDLE_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_AIR_PLAYER), std::vector<uint32_t>{1}, 0);
    AnimationManager::addAnimation(animation, Animation::ID::AIR_PLAYER_IDLE_L);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_AIR_PLAYER), 1, MOVE_ANI_SPEED[Nation::AIR]);
    AnimationManager::addAnimation(animation, Animation::ID::AIR_PLAYER_ATTACK_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_AIR_PLAYER), 2, MOVE_ANI_SPEED[Nation::AIR]);
    AnimationManager::addAnimation(animation, Animation::ID::AIR_PLAYER_ATTACK_L);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_AIR_PLAYER), std::vector<uint32_t>{12}, 0);
    AnimationManager::addAnimation(animation, Animation::ID::AIR_PLAYER_JUMP_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_AIR_PLAYER), std::vector<uint32_t>{13}, 0);
    AnimationManager::addAnimation(animation, Animation::ID::AIR_PLAYER_JUMP_L);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_AIR_PLAYER), 4, MOVE_ANI_SPEED[Nation::AIR]);
    AnimationManager::addAnimation(animation, Animation::ID::AIR_PLAYER_MOV_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_AIR_PLAYER), 5, MOVE_ANI_SPEED[Nation::AIR]);
    AnimationManager::addAnimation(animation, Animation::ID::AIR_PLAYER_MOV_L);

    // WATER PLAYER
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_WATER_PLAYER), std::vector<uint32_t>{0}, 0);
    AnimationManager::addAnimation(animation, Animation::ID::WATER_PLAYER_IDLE_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_WATER_PLAYER), std::vector<uint32_t>{1}, 0);
    AnimationManager::addAnimation(animation, Animation::ID::WATER_PLAYER_IDLE_L);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_WATER_PLAYER), 1, MOVE_ANI_SPEED[Nation::WATER]);
    AnimationManager::addAnimation(animation, Animation::ID::WATER_PLAYER_ATTACK_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_WATER_PLAYER), 2, MOVE_ANI_SPEED[Nation::WATER]);
    AnimationManager::addAnimation(animation, Animation::ID::WATER_PLAYER_ATTACK_L);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_WATER_PLAYER), std::vector<uint32_t>{12}, 0);
    AnimationManager::addAnimation(animation, Animation::ID::WATER_PLAYER_JUMP_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_WATER_PLAYER), std::vector<uint32_t>{13}, 0);
    AnimationManager::addAnimation(animation, Animation::ID::WATER_PLAYER_JUMP_L);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_WATER_PLAYER), 3, MOVE_ANI_SPEED[Nation::WATER]);
    AnimationManager::addAnimation(animation, Animation::ID::WATER_PLAYER_MOV_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_WATER_PLAYER), 4, MOVE_ANI_SPEED[Nation::WATER]);
    AnimationManager::addAnimation(animation, Animation::ID::WATER_PLAYER_MOV_L);

    // ATTACKS
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_FIRE_ATTACK), 1, ATK_ANI_SPEED[Nation::FIRE]);
    AnimationManager::addAnimation(animation, Animation::ID::FIRE_ATTACK_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_FIRE_ATTACK), 0, ATK_ANI_SPEED[Nation::FIRE]);
    AnimationManager::addAnimation(animation, Animation::ID::FIRE_ATTACK_L);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_EARTH_ATTACK), 1, ATK_ANI_SPEED[Nation::EARTH]);
    AnimationManager::addAnimation(animation, Animation::ID::EARTH_ATTACK_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_EARTH_ATTACK), 0, ATK_ANI_SPEED[Nation::EARTH]);
    AnimationManager::addAnimation(animation, Animation::ID::EARTH_ATTACK_L);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_AIR_ATTACK), 1, ATK_ANI_SPEED[Nation::AIR]);
    AnimationManager::addAnimation(animation, Animation::ID::AIR_ATTACK_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_AIR_ATTACK), 0, ATK_ANI_SPEED[Nation::AIR]);
    AnimationManager::addAnimation(animation, Animation::ID::AIR_ATTACK_L);
}

void ResourceLoadingSystem::update(double dt) {
    if(_init){
        _init = false;
        createAnimations();
        t = new std::thread(&ResourceLoadingSystem::loadResources, this);
    }

    if(!_finished_loading || _load_cnt > MAX_LOAD_CNT)
        return;

    if(_load_delay <= 0){
        create_sprite_sheets();
        _load_delay = MAX_LOAD_DELAY;
    }else{
        _load_delay -= dt;
    }
}



