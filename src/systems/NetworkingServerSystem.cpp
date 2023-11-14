#include "systems/NetworkingServerSystem.h"
#include "components/ClientComponent.h"

#include "Kikan/Engine.h"
#include "Kikan/core/Logging.h"


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
        Kikan::Engine::Kikan()->getECS()->getScene()->addEntity(client);
    }

    for (Kikan::Entity *e: _entities) {
        auto *client = e->getComponent<ClientComponent>();
        if(!client)
            return;

        char* buff[6];
        int ret = recv(client->sock_fd, buff, 5, MSG_DONTWAIT);
        if(ret != -1){
            kikanPrint("Player: %s\n", buff);
        }
    }

}
