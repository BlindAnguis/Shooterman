#include "NetworkHandler.h"

#include <cstdint>

#include "../../Common/Trace.h"

NetworkHandler::NetworkHandler() {
  mName = "NETWORK_HANDLER";
}

void NetworkHandler::start() {
  TRACE_INFO("Starting module...");
  mNetworkHandlerThread = std::make_unique<std::thread>(&NetworkHandler::startup, this);
  TRACE_INFO("Starting module done");
}

void NetworkHandler::startup() {
  std::string ip = "10.41.4.19";
  unsigned short port = 1337;
  TRACE_INFO("Connecting socket to " << ip);
  //mSocket.setBlocking(false);
  sf::IpAddress ipAddress = sf::IpAddress(ip);
  sf::Socket::Status status = mSocket.bind(port);// connect(ip, 1337);

  TRACE_INFO("Socket status is: " << status);
  if (status != sf::Socket::Done) {
    TRACE_ERROR("Could not connect to server with address: " << ip);
    mRunning = false;
    return;
  } else {
    TRACE_INFO("Socket connected");
    mRunning = true;
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
      //mSocket.send(packet);
    }

    while (mSocket.receive(packet, ipAddress, port) == sf::Socket::Done) {
      int id = -1;
      packet >> id;
      if (id == SPRITE_LIST) {
        SpriteMessage sm;
        sm.unpack(packet);
        TRACE_DEBUG("Receveid sprite package");
        MessageHandler::get().pushSpriteListMessage(sm.pack());
      } else {
        TRACE_WARNING("Packet not known: " << id);
        mRunning = false;
        break;
      }
    }
    
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