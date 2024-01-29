#include "Kikan/core/Logging.h"


#define LOTUS_PROTOCOL_NAME "LOTUS"
#define HOST_PORT 8888
#define LOTUS_TICK_RATE 60

#define LOTUS_SERVER_BUSY_CODE 42

enum
{
    LOG_INFO,
    LOG_ERROR,
    LOG_DEBUG,
    LOG_TRACE,
    LOG_WARNING
};

#define DISABLED_LOG(...)

#define NBN_LogInfo(...) kikanPrint(__VA_ARGS__)
#define NBN_LogError(...) kikanPrint( __VA_ARGS__)
#define NBN_LogDebug(...) kikanPrint( __VA_ARGS__)
#define NBN_LogTrace(...) DISABLED_LOG( __VA_ARGS__)
#define NBN_LogWarning(...) kikanPrint( __VA_ARGS__)

#define NBNET_IMPL
#include <random>

#include "nbnet/nbnet.h"
#include "nbnet/net_drivers/udp.h"
#include "components/PlayerStateComponent.h"
#include "Kikan/Engine.h"
#include "Kikan/input/Input.h"
#include "components/PlayerComponent.h"
#include "components/EnemyComponent.h"
#include "systems/NetworkSystem.h"
#include "util/GameState.h"
#include "util/Spawner.h"
#include "scenes/Scenes.h"


int InputHistoryIndex = 0;

int LatestNetworkFrame = -1;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> distribution(1000, 9999);
int startPoint = distribution(gen);


static NBN_ConnectionHandle client = 0;

NetworkState NetState = NetworkState::None;

void EchoSleep(double sec)
{
    long nanos = sec * 1e9;
    struct timespec t = {.tv_sec = nanos / 999999999, .tv_nsec = nanos % 999999999};

    nanosleep(&t, &t);

}



bool InitializeHost()
{

    NBN_UDP_Register();
    if(NBN_GameServer_Start(LOTUS_PROTOCOL_NAME, HOST_PORT) < 0)
    {
        kikanPrint("Failed to start server\n");

        return true;
    }

    kikanPrint("Started Hosting\n");
    return false;
}

void InitializeClient(const char *host)
{
    NBN_UDP_Register();
    if (NBN_GameClient_Start(LOTUS_PROTOCOL_NAME, host, HOST_PORT) < 0)
    {
        kikanPrint("Failed to start client");
        exit(-1);
    }
    kikanPrint("Start Client\n");

}

NetworkInputPackage ReadInputMessageCommon(const NBN_MessageInfo& msg_info)
{


    assert(msg_info.type == NBN_BYTE_ARRAY_MESSAGE_TYPE);

    // Retrieve the received message
    auto *msg = (NBN_ByteArrayMessage *)msg_info.data;

    NetworkInputPackage InputPackage;
    memcpy(&InputPackage, msg->bytes, sizeof(NetworkInputPackage));

    // Destroy the received message
    NBN_ByteArrayMessage_Destroy(msg);

    return InputPackage;
}

NetworkInputPackage ReadInputMessage()
{

    // Get info about the received message
    NBN_MessageInfo msg_info = NBN_GameServer_GetMessageInfo();

    return ReadInputMessageCommon(msg_info);

}



NetworkInputPackage ReadInputMessageClient()
{
    // Get info about the received message
    NBN_MessageInfo msg_info = NBN_GameClient_GetMessageInfo();
    return ReadInputMessageCommon(msg_info);

}

void SendInputMessage(NetworkInputPackage &InputPackage)
{

    if (NBN_GameClient_SendUnreliableByteArray((uint8_t *)&InputPackage, sizeof (NetworkInputPackage)) < 0)
        exit(-1);

}

void SendInputMessageHost(NetworkInputPackage &InputPackage)
{
    if (NBN_GameServer_SendUnreliableByteArrayTo(client, (uint8_t *)&InputPackage, sizeof (NetworkInputPackage)))
        exit(-1);
}


NetworkInputPackage TickNetworkHost(NetworkInputPackage InputPackage, bool& bReceivedInput)
{
    bReceivedInput = false;
    if(client)
    {
        std::cout << InputPackage.Name << std::endl;
        SendInputMessageHost(InputPackage);
    }

    NetworkInputPackage ClientInputPackage {  };
    int ev;

    // Poll for server events
    while ((ev = NBN_GameServer_Poll()) != NBN_NO_EVENT) {
        if (ev < 0) {
            kikanPrint( "Something went wrong");

            // Error, quit the server application
            exit(-1);
        }

        switch (ev) {
            // New connection request...
            case NBN_NEW_CONNECTION:
                if (client == 0){
                    NBN_GameServer_AcceptIncomingConnection();
                    client = NBN_GameServer_GetIncomingConnection();

                    std::vector<Kikan::Entity*> players;
                    Kikan::Engine::Kikan()->getECS()->getScene(SCENE_GAME)->getEntities(getSig(PlayerStateComponent), &players);

                    if(players.size() <= 4){
                        auto* enemy = Spawner::spawnPlayer(Nation::EARTH, true);
                        enemy->getComponent<Kikan::Transform>()->position = glm::vec3(100, 800, 0);
                        Kikan::Engine::Kikan()->getECS()->getScene(SCENE_GAME)->addEntity(enemy);
                        players.push_back(enemy);
                    }
                    kikanPrint("Accept client connection \n");
                }

                break;

                // The client has disconnected
                //case NBN_CLIENT_DISCONNECTED:
                //    assert(NBN_GameServer_GetDisconnectedClient() == client->id);

                //    client = NULL;
                //    break;
//
                // A message has been received from the client
            case NBN_CLIENT_MESSAGE_RECEIVED:
                ClientInputPackage = ReadInputMessage();
                bReceivedInput = true;
                break;

        }
    }

    if(NBN_GameServer_SendPackets() < 0)
    {
        kikanPrint("Failed to send packets");
        exit(-1);
    }

    return ClientInputPackage;

}

NetworkInputPackage TickNetworkClient(NetworkInputPackage InputPackage, bool& bReceivedInput)
{
    bReceivedInput = false;

    NetworkInputPackage HostInputPackage {  };
    std::cout << InputPackage.Name << std::endl;

    SendInputMessage(InputPackage);

    int ev;

    // Poll for client events
    while ((ev = NBN_GameClient_Poll()) != NBN_NO_EVENT) {
        if (ev < 0) {
            kikanPrint( "An error occured while polling client events. Exit");

            // Stop main loop
            exit(-1);
        }

        switch (ev) {
            // Client is connected to the server
            case NBN_CONNECTED:
                {
                    std::vector<Kikan::Entity*> players;
                    Kikan::Engine::Kikan()->getECS()->getScene(SCENE_GAME)->getEntities(getSig(PlayerStateComponent), &players);

                    if(players.size() <= 4){
                        auto* enemy = Spawner::spawnPlayer(Nation::EARTH, true);
                        enemy->getComponent<Kikan::Transform>()->position = glm::vec3(100, 800, 0);
                        Kikan::Engine::Kikan()->getECS()->getScene(SCENE_GAME)->addEntity(enemy);
                        players.push_back(enemy);
                    }
                }
                //OnConnected();
                kikanPrint("Connected");

                break;

                // Client has disconnected from the server
                //case NBN_DISCONNECTED:
                //    OnDisconnected();
                //    break;
//
                // A message has been received from the server
            case NBN_MESSAGE_RECEIVED:
                HostInputPackage = ReadInputMessageClient();
                bReceivedInput = true;

                break;
        }
    }

    if(NBN_GameClient_SendPackets() < 0)
    {
        kikanPrint("Failed to send packets");
        exit(-1);
    }

    return HostInputPackage;

}

NetworkSystem::NetworkSystem() {
    includeSingle(PlayerComponent);
    includeSingle(EnemyComponent);
}

void NetworkSystem::update(double dt) {
    double dt2 = 1.0 / LOTUS_TICK_RATE;
    GameState &gameState = GameState::getInstance();
    PlayerStateComponent *player{};
    PlayerStateComponent *enemies{};
    for (Kikan::Entity *e: _entities) {
        auto *p = e->getComponent<PlayerStateComponent>();
        if (p->isEnemy)
            enemies = p;
        else
            player = p;
    }

    NetworkInputPackage LatesInputPackage{};

    bool bReceivedInput = false;

    NetworkInputPackage ToSendNetPackage;
    // Prepare the network package to send to opponent
    {
        const int InputStartIndex = gameState.FrameCount - NET_PACKET_INPUT_HISTORY_SIZE + 1 + NET_INPUT_DELAY;
        //Fill the network package input history with our local input
        std::cout << "Network: " <<player->playerID << std::endl;
        for (int i = 0; i < NET_PACKET_INPUT_HISTORY_SIZE; ++i)
        {
            if (InputStartIndex + i >= 0)
            {
                ToSendNetPackage.InputHistory[i] = gameState.getPlayerInputHistory(player->playerID,
                                                                                   InputStartIndex + i);
            }
            else
            {
                ToSendNetPackage.InputHistory[i] = UINT_MAX;

            }
        }
        ToSendNetPackage.FrameCount = gameState.FrameCount + NET_INPUT_DELAY;
        ToSendNetPackage.PlayerNation = player->nation;
        strcpy(ToSendNetPackage.Name, player->name.c_str());
        ToSendNetPackage.isGameStart = gameState.isGameStart;
        ToSendNetPackage.startPoint = startPoint;
    }

    // Wenn wir Hosten dann setze den Input des Gegners und send unser Input an Client
    if (NetState == NetworkState::Hosting) {
        LatesInputPackage = TickNetworkHost(ToSendNetPackage, bReceivedInput);
    }
        // Wenn wir Client Sind dann setze den Input des Gegners und send unser Input an Client
    else if (NetState == NetworkState::Client) {
        LatesInputPackage = TickNetworkClient(ToSendNetPackage, bReceivedInput);
    }

    if(bReceivedInput)
    {


        // Update network input buffer
        const int StartFrame = LatesInputPackage.FrameCount-NET_PACKET_INPUT_HISTORY_SIZE + 1;
        for (int i = 0; i < NET_PACKET_INPUT_HISTORY_SIZE; ++i)
        {
            const int CheckFrame = StartFrame + i;
            if(CheckFrame  == (LatestNetworkFrame+1))
            {
                LatestNetworkFrame++;
            }
            //std::cout << "Input: " << LatesInputPackage.InputHistory[i] << " Frame: " << LatesInputPackage.FrameCount-NET_PACKET_INPUT_HISTORY_SIZE + i + 1 << std::endl;
            if(enemies)
                gameState.setPlayerInputHistory(enemies->playerID, StartFrame+i, LatesInputPackage.InputHistory[i]);

        }
    }


    // Frame Limiter
    {
        bool isUpdateNextFrame = (gameState.FrameCount == 0);

        if(LatestNetworkFrame > gameState.FrameCount)
        {
            isUpdateNextFrame = true;
        }

        if(NetState == NetworkState::None)
        {
            isUpdateNextFrame = false;
        }

        if (isUpdateNextFrame) {
            std::cout << "Game Ticked[" << gameState.FrameCount << "]" << std::endl;
            gameState.setPlayerInput(player->playerID, gameState.getPlayerInputHistory(player->playerID, gameState.FrameCount + NET_INPUT_DELAY));
            gameState.UpdateGame = true;
            if(enemies) {
                gameState.setPlayerInput(enemies->playerID, gameState.getPlayerInputHistory(enemies->playerID, gameState.FrameCount));
                enemies->nation = LatesInputPackage.PlayerNation;
                enemies->name = std::string(LatesInputPackage.Name);
                enemies->startPoint = LatesInputPackage.startPoint;
                player->startPoint = startPoint;
            }
            gameState.NetFrameCount = LatesInputPackage.FrameCount;
            gameState.FrameCount++;
            gameState.isGameStart = LatesInputPackage.isGameStart;

        }
    }



    if(NetState == NetworkState::None)
    {
        if(gameState.NetState == NetworkState::Hosting)
        {
            InitializeHost();
            NetState = NetworkState::Hosting;
        }else if(gameState.NetState == NetworkState::Client)
        {
            std::cout << gameState.host << std::endl;
            InitializeClient(gameState.host);

            NetState = NetworkState::Client;
        }
    }

    EchoSleep(dt2);



}
