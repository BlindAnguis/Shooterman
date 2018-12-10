#include "NetworkHandler.h"

#include <cstdint>

#include "../../Common/Trace.h"

NetworkHandler::NetworkHandler() {
  mName = "CLIENT: NETWORK_HANDLER";
  mDebugEnabled = true;
}

void NetworkHandler::start() {
  TRACE_INFO("Starting module...");
  mNetworkHandlerThread = std::make_unique<std::thread>(&NetworkHandler::startup, this);
  TRACE_INFO("Starting module done");
}

void NetworkHandler::startup() {
  std::string ip = "10.41.4.93";

  unsigned short port = 1337;
  TRACE_INFO("Connecting socket to " << ip);
  //mTcp = TcpSocket(ip, port);
  //bool connected = mTcp.connect();
  sf::TcpSocket soc;// = sf::TcpSocket();
  auto connected = soc.connect(ip, port);

  // Failed to connect to server
  if (connected != sf::Socket::Status::Done) {
    TRACE_INFO("Connection failed! " << connected);
    GameStateMessage gsm(GAME_STATE::MAIN_MENU);
    MessageHandler::get().pushGameStateMessage(gsm.pack());
    return;
  }
  TRACE_INFO("Connected!");
  MessageHandler::get().subscribeTo("ClientInputList", &mMessageSubscriber);

  while (mRunning) {
    // 1. Receive new port for UDP from server
    // 2. Send port to clientMain
    // 3. Setup connection over UDP

    std::queue<sf::Packet> systemMessageQueue = mMessageSubscriber.getMessageQueue();
    while (!systemMessageQueue.empty()) {
      sf::Packet packet = systemMessageQueue.front();
      systemMessageQueue.pop();
      soc.send(packet);
    }

    sf::Packet packet;
    soc.receive(packet);
    int id = -1;
    packet >> id;
    if (id == SPRITE_LIST) {
      SpriteMessage sm;
      sm.unpack(packet);
      //TRACE_DEBUG("Receveid sprite package");
      MessageHandler::get().pushSpriteListMessage(sm.pack());
    } else {
      TRACE_WARNING("Packet not known: " << id);
      break;
    }
    
    sf::sleep(sf::milliseconds(1));
  }
  MessageHandler::get().unsubscribeAll(&mMessageSubscriber);
}

void NetworkHandler::shutDown() {
  TRACE_INFO("Shutdown of module requested...");
  mRunning = false;
  mNetworkHandlerThread->join();
  TRACE_INFO("Shutdown of module done");
}