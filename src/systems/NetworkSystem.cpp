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
#include "../external/nbnet/nbnet.h"
#include "../external/nbnet/net_drivers/udp.h"
#include "components/PlayerStateComponent.h"
#include "Kikan/Engine.h"
#include "Kikan/input/Input.h"
#include "components/PlayerComponent.h"
#include "components/EnemyComponent.h"
#include "systems/NetworkSystem.h"
#include "util/GameState.h"


int InputHistoryIndex = 0;

int LatestNetworkFrame = -1;



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

void InitializeClient()
{
    NBN_UDP_Register();
    if (NBN_GameClient_Start(LOTUS_PROTOCOL_NAME, "127.0.0.1", HOST_PORT) < 0)
    {
        kikanPrint("Failed to start client");
        exit(-1);
    }
    kikanPrint("Start Client\n");

}

NetworkInputPackage ReadInputMessageCommon(const NBN_MessageInfo& msg_info)
{
    // Get info about the received message
    //NBN_MessageInfo msg_info = NBN_GameClient_GetMessageInfo();

    assert(msg_info.type == NBN_BYTE_ARRAY_MESSAGE_TYPE);

    // Retrieve the received message
    auto *msg = (NBN_ByteArrayMessage *)msg_info.data;

    NetworkInputPackage InputPackage;
    memcpy(&InputPackage, msg->bytes, sizeof(NetworkInputPackage));

    std::cout << "recevied Frame[" << InputPackage.FrameCount << "]" << std::endl;

    // Destroy the received message
    NBN_ByteArrayMessage_Destroy(msg);

    return InputPackage;
}

NetworkInputPackage ReadInputMessage()
{
    //std::cout << "Reading Message" << std::endl;

    // Get info about the received message
    NBN_MessageInfo msg_info = NBN_GameServer_GetMessageInfo();

    //assert(msg_info.sender == client);
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
    std::cout << "sending FrameCount[" << InputPackage.FrameCount << "]" << std::endl;

    if (NBN_GameClient_SendUnreliableByteArray((uint8_t *)&InputPackage, sizeof (NetworkInputPackage)) < 0)
        exit(-1);

}

void SendInputMessageHost(NetworkInputPackage &InputPackage)
{
    std::cout << "sending FrameCount[" << InputPackage.FrameCount << "]" << std::endl;
    if (NBN_GameServer_SendUnreliableByteArrayTo(client, (uint8_t *)&InputPackage, sizeof (NetworkInputPackage)))
        exit(-1);
}


NetworkInputPackage TickNetworkHost(NetworkInputPackage InputPackage, bool& bReceivedInput)
{
    bReceivedInput = false;
    if(client)
    {
        for (int i = 0; i < 5; ++i) {
            SendInputMessageHost(InputPackage);
        }
    }

    NetworkInputPackage ClientInputPackage { 0, 0 };
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
                // Echo server work with one single client at a time
                if (client == 0){
                    NBN_GameServer_AcceptIncomingConnection();
                    client = NBN_GameServer_GetIncomingConnection();
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

    NetworkInputPackage HostInputPackage { 0, 0 };

    for (int i = 0; i < 5; ++i) {
        SendInputMessage(InputPackage);
    }
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
    PlayerStateComponent *player;
    PlayerStateComponent *enemies;
    for (Kikan::Entity *e: _entities) {
        auto *p = e->getComponent<PlayerStateComponent>();
        //todo do it for all enemies (make a map or vector out of it)
        if (p->isEnemy)
            enemies = p;
        else if (!p->isEnemy)
            player = p;
        //auto *enemy = e->getComponent<EnemyComponent>();
    }
    //todo loop durch every enemy to reciev and send input

    NetworkInputPackage LatesInputPackage{};

    bool bReceivedInput = false;

    NetworkInputPackage ToSendNetPackage;
    // Prepare the network package to send to opponent (maybe just do this if your not an enemy???)
    {
        const int InputStartIndex = gameState.FrameCount - NET_PACKET_INPUT_HISTORY_SIZE + 1 + NET_INPUT_DELAY;
        //Fill the network package input history with our local input
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
    }


    // Wenn wir Hosten dann setze den Input des Gegners und send unser Input an Client
    if (NetState == NetworkState::Hosting) {
        LatesInputPackage = TickNetworkHost(ToSendNetPackage, bReceivedInput);
    }
        // Wenn wir Client Sind dann setze den Input des Gegners und send unser Input an Client
    else if (NetState == NetworkState::Client) {
        LatesInputPackage = TickNetworkClient(ToSendNetPackage, bReceivedInput);
    }


    //for (const auto& InputPackage: NetworkInputHistory) {
    //    if (InputPackage.FrameCount == LatesInputPackage.FrameCount) {
    //        bReceivedInput = true;
    //        break;
    //    }
    //}

    if(bReceivedInput)
    {
        std::cout << "Received Net Input: Frame[" << LatesInputPackage.FrameCount << "]" << std::endl;
        // Update network input buffer
        //NetworkInputHistory[InputHistoryIndex] = LatesInputPackage;
        //InputHistoryIndex = (InputHistoryIndex + 1) % INPUT_HISTORY_SIZE;
        const int StartFrame = LatesInputPackage.FrameCount-NET_PACKET_INPUT_HISTORY_SIZE + 1;
        for (int i = 0; i < NET_PACKET_INPUT_HISTORY_SIZE; ++i)
        {
            const int CheckFrame = StartFrame + i;
            if(CheckFrame  == (LatestNetworkFrame+1))
            {
                LatestNetworkFrame++;
            }
            //std::cout << "Input: " << LatesInputPackage.InputHistory[i] << " Frame: " << LatesInputPackage.FrameCount-NET_PACKET_INPUT_HISTORY_SIZE + i + 1 << std::endl;

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
            gameState.setPlayerInput(enemies->playerID, gameState.getPlayerInputHistory(enemies->playerID, gameState.FrameCount));
            gameState.NetFrameCount = LatesInputPackage.FrameCount;
            gameState.FrameCount++;

        }
    }



    if(NetState == NetworkState::None)
    {
        if(Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::O))
        {
            InitializeHost();
            NetState = NetworkState::Hosting;
        }else if(Kikan::Engine::Kikan()->getInput()->keyPressed(Kikan::Key::C))
        {
            InitializeClient();

            NetState = NetworkState::Client;
        }
    }

    EchoSleep(dt2);



}
