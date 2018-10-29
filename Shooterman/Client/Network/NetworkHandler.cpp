#include "NetworkHandler.h"

#include <cstdint>

#include "../../Common/Trace.h"

NetworkHandler::NetworkHandler() {
  mName = "NETWORK_HANDLER";
  mDebugEnabled = true;
}

void NetworkHandler::start() {
  TRACE_INFO("Starting module...");
  mNetworkHandlerThread = std::make_unique<std::thread>(&NetworkHandler::startup, this);
  TRACE_INFO("Starting module done");
}

void NetworkHandler::startup() {
  std::string ip = "10.41.4.122";
  unsigned short port = 1337;
  TRACE_INFO("Connecting socket to " << ip);

  bool connected = mTcp.connect(ip, port);

  if (!connected) {
    GameStateMessage gsm(GAME_STATE::MAIN_MENU);
    MessageHandler::get().pushGameStateMessage(gsm.pack());
    return;
  }

  GameStateMessage gsm(GAME_STATE::PLAYING);
  MessageHandler::get().pushGameStateMessage(gsm.pack());
  MessageHandler::get().subscribeToSystemMessages(&mMessageSubscriber);
  MessageHandler::get().subscribeToGameStateMessages(&mMessageSubscriber);
  MessageHandler::get().subscribeToInputMessages(&mMessageSubscriber);


  while (mRunning) {
    TRACE_INFO("Listening to socket");
    std::queue<sf::Packet> systemMessageQueue = mMessageSubscriber.getMessageQueue();
    sf::Packet packet;
    while (!systemMessageQueue.empty()) {
      packet = systemMessageQueue.front();
      systemMessageQueue.pop();
      //mUdpSocket.send(packet);
    }

    readTcp();
    
    sf::sleep(sf::milliseconds(FRAME_LENGTH_IN_MS));
  }
  MessageHandler::get().unsubscribeAll(&mMessageSubscriber);
}

void NetworkHandler::shutDown() {
  TRACE_INFO("Shutdown of module requested...");
  mRunning = false;
  mNetworkHandlerThread->join();
  TRACE_INFO("Shutdown of module done");
}

void NetworkHandler::readTcp() {
  std::vector<sf::Packet> packets = mTcp.read();
  for (auto packet : packets) {
    int id = -1;
    packet >> id;
    if (id == SPRITE_LIST) {
      SpriteMessage sm;
      sm.unpack(packet);
      TRACE_DEBUG("Receveid sprite package");
      MessageHandler::get().pushSpriteListMessage(sm.pack());
    }
    else {
      TRACE_WARNING("Packet not known: " << id);
      break;
    }
  }
}

void NetworkHandler::readUdp() {
  std::vector<sf::Packet> packets = mUdp.read();
  for (auto packet : packets) {
    int id = -1;
    packet >> id;
    if (id == SPRITE_LIST) {
      SpriteMessage sm;
      sm.unpack(packet);
      TRACE_DEBUG("Receveid sprite package");
      MessageHandler::get().pushSpriteListMessage(sm.pack());
    }
    else {
      TRACE_WARNING("Packet not known: " << id);
      break;
    }
  }
}