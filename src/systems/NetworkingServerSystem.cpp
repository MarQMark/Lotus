#include "systems/NetworkingServerSystem.h"
#include "components/ClientComponent.h"

#include "Kikan/Engine.h"
#include "Kikan/core/Logging.h"
#include "networking/Messages.h"


#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>


NetworkingServerSystem::NetworkingServerSystem() {
    singleInclude(ClientComponent);
}

static int createPassiveSocket(in_port_t port)
{
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    int flags = fcntl(sock_fd, F_GETFL, 0);
    fcntl(sock_fd, F_SETFL, flags | O_NONBLOCK);

    const int on = 1;
    setsockopt(sock_fd, SOL_SOCKET , SO_REUSEADDR , &on, sizeof(on));

    struct sockaddr_in socaddr;
    socaddr.sin_family = AF_INET;
    socaddr.sin_port = htons(port);
    socaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sock_fd, (struct sockaddr *)&socaddr, sizeof(socaddr)) == -1){
        close(sock_fd);
        return -1;
    }

    if(listen(sock_fd, 6) == -1){
        close(sock_fd);
        return -1;
    }

    return sock_fd;
}


void NetworkingServerSystem::update(double dt) {
    if(!_enabled){
        _enabled = Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::O);
        if(_enabled){
            kikanPrint("Server Started\n");

            _passive_sock = createPassiveSocket(8888);
            if(_passive_sock == -1){
                kikanPrintE("Could not create server socket\n");
            }
        }
        return;
    }

    // Check for new Client
    int sock = accept(_passive_sock, NULL, NULL);
    if(sock != -1){
        kikanPrint("Player joined\n");
        auto* client = new Kikan::Entity();
        auto* clientComp = new ClientComponent();
        clientComp->sock_fd = sock;
        client->addComponent(clientComp);
        addEntity(client);
    }

    std::vector<PlayerData> dataMsgs;

    for (Kikan::Entity *e: _entities) {
        auto *client = e->getComponent<ClientComponent>();
        if(!client)
            return;

        Message msg{};
        int ret;

        if(!client->auth){
            ret = recv(client->sock_fd, &msg, sizeof(Header), MSG_DONTWAIT);
            if(sizeof(Header) != ret){
                kikanPrint("Server: Invalid header length: %d\n", ret);
                continue;
            }

            ret = recv(client->sock_fd, &msg.body, sizeof(C2SAuth), MSG_DONTWAIT);
            if(ret != msg.hdr.len){
                kikanPrint("Server: Invalid body length: %d\n", ret);
                continue;
            }
            if(msg.hdr.id != MessageID::C2SAuth){
                kikanPrint("Server: Expected C2S_Auth but received: %d\n", msg.hdr.id);
                continue;
            }

            client->id = _num_players;
            msg.hdr.id = MessageID::S2CAuth;
            msg.hdr.len = sizeof(S2C_Auth);
            msg.body.s2c_auth.playerID = _num_players;
            send(client->sock_fd, &msg, MESSAGE_SIZE(msg), MSG_NOSIGNAL);

            // Inform all other players that new player joined
            for (Kikan::Entity *e2: _entities) {
                auto *client2 = e2->getComponent<ClientComponent>();
                if (!client2)
                    return;

                if(!client2->auth)
                    continue;

                msg.hdr.id = S2CJoin;
                msg.hdr.len = sizeof(S2C_Join);
                msg.body.s2c_join.playerID = _num_players;
                send(client2->sock_fd, &msg, MESSAGE_SIZE(msg), MSG_NOSIGNAL);
            }

            client->auth = true;
            _num_players++;

            continue;
        }

        // receive default data
        ret = recv(client->sock_fd, &msg, sizeof(Header), MSG_DONTWAIT);
        if(ret == -1)
            continue;
        if(ret != sizeof(Header)){
            kikanPrint("Server: Invalid Header with size: %d\n", ret);
            continue;
        }
        ret = recv(client->sock_fd, &msg.body, msg.hdr.len, MSG_DONTWAIT);
        if(ret != msg.hdr.len){
            continue;
        }
        else{
            client->pos = glm::vec2(msg.body.c2s_pos.x, msg.body.c2s_pos.y);
            client->initData = true;

            if(client->auth){
                PlayerData data {
                        .id = (uint16_t)client->id,
                        .x = client->pos.x,
                        .y = client->pos.y,
                };
                dataMsgs.push_back(data);
            }
        }
    }

    // send position of all players to all authed players
    if(!dataMsgs.empty()){
        Header hdr;
        hdr.id = S2CData;
        hdr.len = sizeof(PlayerData) * dataMsgs.size();

        void* data = malloc(sizeof(Header) + hdr.len);
        memcpy(data, &hdr, sizeof(Header));
        memcpy((uint8_t*)data + sizeof(Header), dataMsgs.data(), hdr.len);

        for (Kikan::Entity *e: _entities) {
            auto *client = e->getComponent<ClientComponent>();
            if (!client)
                continue;

            if(!client->initData)
                continue;

            send(client->sock_fd, data, sizeof(Header) + hdr.len, MSG_NOSIGNAL);
        }

        free(data);
    }
}
