#include "NetworkHandler.h"

#include <cstdint>

#include "../../Common/Trace.h"

NetworkHandler::NetworkHandler() {
  mName = "NETWORK_HANDLER";
}

void NetworkHandler::start() {
  TRACE_INFO("Starting module...");
  mNetworkHandlerThread = new std::thread(&NetworkHandler::startup, this);
  TRACE_INFO("Starting module done");
}

void NetworkHandler::startup() {
  std::string ip = "localhost";
  TRACE_INFO("Connecting socket to " << ip);
  mSocket.setBlocking(false);
  sf::IpAddress ipAddress = sf::IpAddress::getLocalAddress();
  sf::Socket::Status status = mSocket.connect(ipAddress, 1337);
  TRACE_INFO("Socket connected");

  if (status != sf::Socket::Done) {
    TRACE_ERROR("Could not connect to server");
    mRunning = false;
    return;
  } else {
    mRunning = true;
  }

  GameStateMessage gsm(GAME_STATE::PLAYING);
  MessageHandler::get().pushGameStateMessage(gsm.pack());
  MessageHandler::get().subscribeToSystemMessages(&mMessageSubscriber);
  MessageHandler::get().subscribeToGameStateMessages(&mMessageSubscriber);
  MessageHandler::get().subscribeToInputMessages(&mMessageSubscriber);

  TRACE_INFO("Listening to socket");

  while (mRunning) {
    std::queue<sf::Packet> systemMessageQueue = mMessageSubscriber.getMessageQueue();
    sf::Packet packet;
    while (!systemMessageQueue.empty()) {
      packet = systemMessageQueue.front();
      systemMessageQueue.pop();
      mSocket.send(packet);
    }

    while (mSocket.receive(packet) != sf::Socket::NotReady) {
      TRACE_INFO("RECEVIED PACKAGE");
    }

    sf::sleep(sf::milliseconds(FRAME_LENGTH_IN_MS));
  }
}

void NetworkHandler::shutDown() {
  TRACE_INFO("Shutdown of module requested...");
  mRunning = false;
  mNetworkHandlerThread->join();
  delete mNetworkHandlerThread;
  TRACE_INFO("Shutdown of module done");
}