#include <sstream>
#include "systems/GameLoopSystem.h"
#include "components/PlayerStateComponent.h"
#include "components/HealthComponent.h"
#include "Kikan/ecs/components/Physics.h"
#include "Kikan/Engine.h"
#include "scenes/Scenes.h"
#include "Kikan/ui/elements/Label.h"
#include "string"

double GameLoopSystem::Countdown;

GameLoopSystem::GameLoopSystem() {
    includeSingle(PlayerStateComponent);
}

void GameLoopSystem::update(double dt) {
    uint32_t aliveCnt = 0;

    if(Countdown >= 0){
        auto* countdown = (Kikan::Label*)Kikan::Engine::Kikan()->getUI()->getElement("Countdown");

        if(Countdown >= 1000){
            std::stringstream ss;
            ss << " ";
            ss << ((int)(Countdown / 1000));
            countdown->setText(ss.str());
        } else{
            countdown->setText("Fight");
        }

        Countdown -= dt;
        if(Countdown < 0){
            countdown->enabled = false;
            for (auto* e : _entities) {
                auto* player = e->getComponent<PlayerStateComponent>();
                player->canInput = true;
            }
        }
    }


    for (auto* e : _entities) {
        auto* player = e->getComponent<PlayerStateComponent>();
        if(Countdown >= 0)
            player->canInput = false;

        auto* health = e->getComponent<HealthComponent>();
        if(!health)
            continue;

        if(health->health > 0){
            aliveCnt++;
        }
        else{
            auto* transform = e->getComponent<Kikan::Transform>();
            //This is stupid. Too bad!
            transform->position = glm::vec3(487123864.f);

            auto* physics = e->getComponent<Kikan::Physics>();
            if(physics){
                physics->velocity = glm::vec2(0);
                physics->acceleration = glm::vec2(0);
            }

            player->canInput = false;
        }
    }

    if(aliveCnt <= 1){
        Kikan::Engine::Kikan()->getECS()->loadScene(SCENE_MENU);
        Kikan::Engine::Kikan()->getUI()->getNode(UI_VICTORY)->enabled = true;
    }
}
