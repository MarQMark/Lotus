#include "systems/NetworkingClientSystem.h"
#include "components/NetworkingComponent.h"

#include "Kikan/Engine.h"
#include "Kikan/core/Logging.h"

#include <sys/socket.h>
#include <netinet/in.h>

NetworkingClientSystem::NetworkingClientSystem() {
    singleInclude(NetworkingComponent);
}

void NetworkingClientSystem::update(double dt) {

    if(!_enabled){
        _enabled = Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::C);
        if(_enabled){
            kikanPrint("Client Connecting...\n");
            // Creating socket file descriptor
            if ( (_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
                perror("socket creation failed");
                exit(EXIT_FAILURE);
            }

            struct sockaddr_in servaddr;

            // Filling server information
            servaddr.sin_family = AF_INET;
            servaddr.sin_port = htons(8888);
            servaddr.sin_addr.s_addr = INADDR_ANY;

            connect(_sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
            kikanPrint("Message send\n");
        }
        return;
    }

    if(_sockfd != -1){
        if(Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::M)){
            char* buff = "Hello";
            send(_sockfd, buff, 6, 0);
        }
    }

    for (Kikan::Entity *e: _entities) {
        auto *network = e->getComponent<NetworkingComponent>();
        if(!network)
            return;



    }
}
