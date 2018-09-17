#include "NetworkHandler.h"

#include <cstdint>

#include "../../Common/Trace.h"

NetworkHandler::NetworkHandler()
{
  mName = "NETWORK_HANDLER";
  TRACE_INFO("Starting module...");
  mNetworkHandlerThread = new std::thread(&NetworkHandler::startup, this);
  TRACE_INFO("Starting module done");
}

void NetworkHandler::startup() {
  MessageHandler::get().subscribeToSystemMessages(&mMessageSubscriber);

  mSocket.setBlocking(false);
  sf::Socket::Status status = mSocket.connect("localhost", 1337);

  if (status != sf::Socket::Done) {
    TRACE_ERROR("Could not connect to server");
    mRunning = false;
  } else {
    mRunning = true;
  }

  while (mRunning) {
    std::queue<sf::Packet> systemMessageQueue = mMessageSubscriber.getMessageQueue();
    sf::Packet packet;
    while (!systemMessageQueue.empty()) {
      packet = systemMessageQueue.front();
      systemMessageQueue.pop();
      mSocket.send(packet);
    }

    while (mSocket.receive(packet) != sf::Socket::NotReady) {
      TRACE_INFO("RECEVIED PACAGE");
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