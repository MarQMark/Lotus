#include "systems/NetworkingClientSystem.h"
#include "components/NetworkingComponent.h"

#include "Kikan/Engine.h"
#include "Kikan/core/Logging.h"
#include "components/PlayerComponent.h"
#include "Kikan/ecs/components/LineQuadSprite.h"
#include "networking/Messages.h"
#include "components/EnemyComponent.h"
#include "util/Spawner.h"
#include "components/MessageComponent.h"
#include "Kikan/ecs/components/Physics.h"
#include "components/TriggerComponent.h"

#include <sys/socket.h>
#include <netinet/in.h>


NetworkingClientSystem::NetworkingClientSystem() {
    includeSingle(PlayerComponent);
    includeSingle(MessageComponent);
}

void NetworkingClientSystem::addEntity(Kikan::Entity *entity) {
    if (entity->getComponent<MessageComponent>()){
        _m_queue.push(entity);
        return;
    }

    ISystem::addEntity(entity);
    if(entity->getComponent<PlayerComponent>()){
        _player = entity;
    }
}

void NetworkingClientSystem::removeEntity(Kikan::Entity *entity) {
    ISystem::removeEntity(entity);
}

void NetworkingClientSystem::addEnemy(uint16_t id){
    auto* enemy = Spawner::spawnEnemy();

    enemy->getComponent<Kikan::Transform>()->position = glm::vec3(80, 80, 0);
    enemy->getComponent<EnemyComponent>()->enemyID = id;
    Kikan::Engine::Kikan()->getECS()->getScene()->addEntity(enemy);

    _enemies[id] = enemy;
}

void NetworkingClientSystem::update(double dt) {

    if(!_enabled){
        _enabled = Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::C);
        if(_enabled){
            kikanPrint("Client Connecting...\n");
            // Creating socket file descriptor
            if ( (_sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
                perror("socket creation failed");
                exit(EXIT_FAILURE);
            }

            struct sockaddr_in serv_addr{};

            // Filling server information
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(8888);
            serv_addr.sin_addr.s_addr = INADDR_ANY;

            int ret = connect(_sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
            if(ret == -1){
                _enabled = false;
                kikanPrintE("Client: Error could not connect to Server\n");
                return;
            }

            Message msg{};
            msg.hdr.id = MessageID::C2SAuth;
            msg.hdr.len = sizeof(C2S_Auth);
            send(_sock_fd, &msg, MESSAGE_SIZE(msg), MSG_NOSIGNAL);
        }
        return;
    }

    if(_sock_fd == -1)
        return;

    if(!_auth){
        Message msg{};
        int ret = recv(_sock_fd, &msg, sizeof(Header), MSG_DONTWAIT);
        if(sizeof(Header) != ret){
            kikanPrint("Client: Invalid Header received with length %d\n", ret);
            return;
        }
        if(msg.hdr.id != MessageID::S2CAuth){
            kikanPrint("Client: Unexpected message received with type %d, expected S2C_Auth with id %d\n", ret, MessageID::S2CAuth);
            return;

        }
        ret = recv(_sock_fd, &msg.body, sizeof(Body), MSG_DONTWAIT);
        if(ret != msg.hdr.len){
            kikanPrintE("Client: Body with unexpected length received: %d\n", ret);
            return;
        }

        auto* playerComp = _player->getComponent<PlayerComponent>();
        if(!playerComp){
            kikanPrintE("Client: Error: player has no playerComponent\n");
            return;
        }

        playerComp->playerID = msg.body.s2c_auth.playerID;
        for(uint16_t i = 0; i < msg.body.s2c_auth.playerID; i++)
            addEnemy(i);

        _auth = true;
        kikanPrintE("Client: Auth successful\n");
    }

    //for (Kikan::Entity *e: _entities)
    {
        auto *transform = _player->getComponent<Kikan::Transform>();

        Message msg{};
        msg.hdr.id = MessageID::C2SPos;
        msg.hdr.len = sizeof(C2S_Pos);
        msg.body.c2s_pos.x = transform->position.x;
        msg.body.c2s_pos.y = transform->position.y;

        send(_sock_fd, &msg, MESSAGE_SIZE(msg), MSG_NOSIGNAL);
    }

    // send all queued messages
    while(!_m_queue.empty()){
        auto* entity = _m_queue.front();
        auto msgComponent = entity->getComponent<MessageComponent>();
        send(_sock_fd, &msgComponent->msg, MESSAGE_SIZE(msgComponent->msg), MSG_NOSIGNAL);
        _m_queue.pop();
        delete entity;
    }

    Message msg{};
    while(true){

        int ret = recv(_sock_fd, &msg, sizeof(Header), MSG_DONTWAIT);
        if(ret == -1){
            return;
        }
        if(ret != sizeof(Header)){
            kikanPrint("Client: Invalid header length: %d\n", ret);
            return;
        }

        if(msg.hdr.id == MessageID::S2CJoin){
            ret = recv(_sock_fd, &msg.body, sizeof(S2C_Join), MSG_DONTWAIT);
            if(ret != msg.hdr.len){
                kikanPrint("Client: Invalid body length: %d\n", ret);
                return;
            }
            if(msg.body.s2c_join.playerID == _player->getComponent<PlayerComponent>()->playerID){
                kikanPrint("Client: Received join for own ID\n");
            }

            addEnemy(msg.body.s2c_join.playerID);
        }
        else if(msg.hdr.id == MessageID::S2CData){
            PlayerData playerData[msg.hdr.len / sizeof(PlayerData)];
            ret = recv(_sock_fd, playerData, msg.hdr.len, MSG_DONTWAIT);
            if(ret == -1)
                return;

            for(auto data : playerData){
                if(data.id == _player->getComponent<PlayerComponent>()->playerID)
                    continue;

                if(!_enemies[data.id])
                    continue;

                auto transform2 = _enemies[data.id]->getComponent<Kikan::Transform>();
                transform2->position.x = data.x;
                transform2->position.y = data.y;
            }
        }
        else if(msg.hdr.id == MessageID::Attack){
            ret = recv(_sock_fd, &msg.body, sizeof(BAttack), MSG_DONTWAIT);
            if(ret != msg.hdr.len){
                kikanPrint("Client: Invalid body length: %d\n", ret);
                return;
            }
            if(msg.body.s2c_join.playerID == _player->getComponent<PlayerComponent>()->playerID){
                kikanPrint("Client: Received join for own ID\n");
            }

            auto* attack = Spawner::spawnAttack(glm::vec2(msg.body.attack.x, msg.body.attack.y));
            if(msg.body.attack.direction == 1){
                attack->getComponent<Kikan::Physics>()->velocity.x *= -1;
                attack->getComponent<TriggerComponent>()->impulse.x *= -1;
            }
            Kikan::Engine::Kikan()->getECS()->getScene()->addEntity(attack);
        }

    }
}


