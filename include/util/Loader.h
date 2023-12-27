#ifndef LOTUS_LOADER_H
#define LOTUS_LOADER_H

void loadTextures(){
    ResourceManager::add<TextureResource>(new TextureResource("res/Maps/OuterWall2/background.png"), Resource::ID::TEX_OUTER_WALL_BACKGROUND);
    ResourceManager::add<TextureResource>(new TextureResource("res/Maps/OuterWall2/foreground.png"), Resource::ID::TEX_OUTER_WALL_FOREGROUND);
    ResourceManager::add<TextureResource>(new TextureResource("res/Maps/OuterWall2/Clouds.png"),     Resource::ID::TEX_OUTER_WALL_CLOUDS);
    ResourceManager::add<TextureResource>(new TextureResource("res/Maps/OuterWall2/Train.png"),      Resource::ID::TEX_OUTER_WALL_TRAIN);
}

void loadSpriteSheets(){
    SpriteSheetResource* spriteSheet;

    // Players
    spriteSheet = new SpriteSheetResource("res/Fire/FirePlayerSpriteSheet.png");
    spriteSheet->addGrid(650, 1200);
    ResourceManager::add<SpriteSheetResource>(spriteSheet, Resource::ID::SS_FIRE_PLAYER);
    spriteSheet = new SpriteSheetResource("res/Earth/EarthPlayerSpriteSheet.png");
    spriteSheet->addGrid(650, 1200);
    ResourceManager::add<SpriteSheetResource>(spriteSheet, Resource::ID::SS_EARTH_PLAYER);
    spriteSheet = new SpriteSheetResource("res/Air/AirPlayerSpriteSheet.png");
    spriteSheet->addGrid(650, 1200);
    ResourceManager::add<SpriteSheetResource>(spriteSheet, Resource::ID::SS_AIR_PLAYER);
    // TODO: Change to actual water player once finished
    spriteSheet = new SpriteSheetResource("res/Fire/FirePlayerSpriteSheet.png");
    spriteSheet->addGrid(650, 1200);
    ResourceManager::add<SpriteSheetResource>(spriteSheet, Resource::ID::SS_WATER_PLAYER);

    // Attacks
    spriteSheet = new SpriteSheetResource("res/Fire/fireballSprite.png");
    spriteSheet->addGrid(300, 150);
    ResourceManager::add<SpriteSheetResource>(spriteSheet, Resource::ID::SS_FIRE_ATTACK);
    spriteSheet = new SpriteSheetResource("res/Earth/RockSprite.png");
    spriteSheet->addGrid(340, 180);
    ResourceManager::add<SpriteSheetResource>(spriteSheet, Resource::ID::SS_EARTH_ATTACK);
    spriteSheet = new SpriteSheetResource("res/Air/AirAttack.png");
    spriteSheet->addGrid(195, 485);
    ResourceManager::add<SpriteSheetResource>(spriteSheet, Resource::ID::SS_AIR_ATTACK);

    // Healthbar
    spriteSheet = new SpriteSheetResource("res/Assets/SpriteSheetHealthbarEnemy.png");
    spriteSheet->addGrid(400, 50);
    ResourceManager::add<SpriteSheetResource>(spriteSheet, Resource::ID::SS_HEALTHBAR_ENEMY);
}

// TODO: Make better
void createAnimations(){
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
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_FIRE_PLAYER), 3, FIRE_MOV_ANI_SPEED);
    AnimationManager::addAnimation(animation, Animation::ID::FIRE_PLAYER_MOV_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_FIRE_PLAYER), 4, FIRE_MOV_ANI_SPEED);
    AnimationManager::addAnimation(animation, Animation::ID::FIRE_PLAYER_MOV_L);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_FIRE_PLAYER), 5, FIRE_MOV_ANI_SPEED);
    AnimationManager::addAnimation(animation, Animation::ID::FIRE_PLAYER_JUMP_ABL_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_FIRE_PLAYER), 6, FIRE_MOV_ANI_SPEED);
    AnimationManager::addAnimation(animation, Animation::ID::FIRE_PLAYER_JUMP_ABL_L);

    // EARTH PLAYER
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_EARTH_PLAYER), std::vector<uint32_t>{0}, 0);
    AnimationManager::addAnimation(animation, Animation::ID::EARTH_PLAYER_IDLE_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_EARTH_PLAYER), std::vector<uint32_t>{1}, 0);
    AnimationManager::addAnimation(animation, Animation::ID::EARTH_PLAYER_IDLE_L);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_EARTH_PLAYER), 1, EARTH_ATTACK_ANI_SPEED);
    AnimationManager::addAnimation(animation, Animation::ID::EARTH_PLAYER_ATTACK_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_EARTH_PLAYER), 2, EARTH_ATTACK_ANI_SPEED);
    AnimationManager::addAnimation(animation, Animation::ID::EARTH_PLAYER_ATTACK_L);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_EARTH_PLAYER), std::vector<uint32_t>{12}, 0);
    AnimationManager::addAnimation(animation, Animation::ID::EARTH_PLAYER_JUMP_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_EARTH_PLAYER), std::vector<uint32_t>{13}, 0);
    AnimationManager::addAnimation(animation, Animation::ID::EARTH_PLAYER_JUMP_L);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_EARTH_PLAYER), 4, EARTH_MOV_ANI_SPEED);
    AnimationManager::addAnimation(animation, Animation::ID::EARTH_PLAYER_MOV_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_EARTH_PLAYER), 5, EARTH_MOV_ANI_SPEED);
    AnimationManager::addAnimation(animation, Animation::ID::EARTH_PLAYER_MOV_L);

    // AIR PLAYER
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_AIR_PLAYER), std::vector<uint32_t>{0}, 0);
    AnimationManager::addAnimation(animation, Animation::ID::AIR_PLAYER_IDLE_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_AIR_PLAYER), std::vector<uint32_t>{1}, 0);
    AnimationManager::addAnimation(animation, Animation::ID::AIR_PLAYER_IDLE_L);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_AIR_PLAYER), 1, AIR_ATTACK_ANI_SPEED);
    AnimationManager::addAnimation(animation, Animation::ID::AIR_PLAYER_ATTACK_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_AIR_PLAYER), 2, AIR_ATTACK_ANI_SPEED);
    AnimationManager::addAnimation(animation, Animation::ID::AIR_PLAYER_ATTACK_L);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_AIR_PLAYER), std::vector<uint32_t>{12}, 0);
    AnimationManager::addAnimation(animation, Animation::ID::AIR_PLAYER_JUMP_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_AIR_PLAYER), std::vector<uint32_t>{13}, 0);
    AnimationManager::addAnimation(animation, Animation::ID::AIR_PLAYER_JUMP_L);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_AIR_PLAYER), 4, AIR_MOV_ANI_SPEED);
    AnimationManager::addAnimation(animation, Animation::ID::AIR_PLAYER_MOV_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_AIR_PLAYER), 5, AIR_MOV_ANI_SPEED);
    AnimationManager::addAnimation(animation, Animation::ID::AIR_PLAYER_MOV_L);

    // WATER PLAYER
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_WATER_PLAYER), std::vector<uint32_t>{0}, 0);
    AnimationManager::addAnimation(animation, Animation::ID::WATER_PLAYER_IDLE_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_WATER_PLAYER), std::vector<uint32_t>{1}, 0);
    AnimationManager::addAnimation(animation, Animation::ID::WATER_PLAYER_IDLE_L);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_WATER_PLAYER), 1, WATER_ATTACK_ANI_SPEED);
    AnimationManager::addAnimation(animation, Animation::ID::WATER_PLAYER_ATTACK_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_WATER_PLAYER), 2, WATER_ATTACK_ANI_SPEED);
    AnimationManager::addAnimation(animation, Animation::ID::WATER_PLAYER_ATTACK_L);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_WATER_PLAYER), std::vector<uint32_t>{12}, 0);
    AnimationManager::addAnimation(animation, Animation::ID::WATER_PLAYER_JUMP_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_WATER_PLAYER), std::vector<uint32_t>{13}, 0);
    AnimationManager::addAnimation(animation, Animation::ID::WATER_PLAYER_JUMP_L);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_WATER_PLAYER), 3, WATER_MOV_ANI_SPEED);
    AnimationManager::addAnimation(animation, Animation::ID::WATER_PLAYER_MOV_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_WATER_PLAYER), 4, WATER_MOV_ANI_SPEED);
    AnimationManager::addAnimation(animation, Animation::ID::WATER_PLAYER_MOV_L);

    // ATTACKS
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_FIRE_ATTACK), 1, FIRE_ATK_ANI_SPEED);
    AnimationManager::addAnimation(animation, Animation::ID::FIRE_ATTACK_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_FIRE_ATTACK), 0, FIRE_ATK_ANI_SPEED);
    AnimationManager::addAnimation(animation, Animation::ID::FIRE_ATTACK_L);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_EARTH_ATTACK), 1, EARTH_ATK_ANI_SPEED);
    AnimationManager::addAnimation(animation, Animation::ID::EARTH_ATTACK_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_EARTH_ATTACK), 0, EARTH_ATK_ANI_SPEED);
    AnimationManager::addAnimation(animation, Animation::ID::EARTH_ATTACK_L);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_AIR_ATTACK), 1, AIR_ATK_ANI_SPEED);
    AnimationManager::addAnimation(animation, Animation::ID::AIR_ATTACK_R);
    animation = new Animation(ResourceManager::get<SpriteSheetResource>(Resource::ID::SS_AIR_ATTACK), 0, AIR_ATK_ANI_SPEED);
    AnimationManager::addAnimation(animation, Animation::ID::AIR_ATTACK_L);
}

#endif //LOTUS_LOADER_H
