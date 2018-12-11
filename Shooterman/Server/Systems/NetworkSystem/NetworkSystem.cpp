#include "NetworkSystem.h"


NetworkSystem::NetworkSystem() {
  mName = "SERVER: NETWORK_SYSTEM";
  mDebugEnabled = true;
  mMapLock = new std::mutex();
}

NetworkSystem::~NetworkSystem() {
  delete mMapLock;
}

void NetworkSystem::start() {
  TRACE_INFO("Starting module...");
  mNetworkSystemThread = std::make_unique<std::thread>(&NetworkSystem::startup, this);
  TRACE_INFO("Starting module done");
}

void NetworkSystem::startup() {
  mRunning = true;
  mClientsSockets.clear();
  mNewClientsSockets.clear();
  while (mRunning) {
    updateInternalMap();
    for (auto client : mClientsSockets) {
      client.second->setBlocking(false);
      sf::Packet packet;
      if (client.second->receive(packet) != sf::Socket::Done) {
        //TRACE_ERROR("Could not read socket data");
        //removeClientSocket(client.first);
      } else {
        int packetId = -1;
        packet >> packetId;
        switch (packetId)
        {
        case INPUT_KEYS:
        {
          packet << client.first;
          MessageHandler::get().pushInputMessage(packet);
          break;
        }
        case SHUT_DOWN:
          break;
        case CHANGE_GAME_STATE:
          break;
        default:
          TRACE_ERROR("Received unhandled packet with ID: " << packetId);
          break;
        }
      }
    }

    //sf::sleep(sf::milliseconds(FRAME_LENGTH_IN_MS));
  }

  for (auto client : mClientsSockets) {
    client.second->disconnect();
    delete client.second;
  }
}

void NetworkSystem::shutDown() {
  TRACE_INFO("Shutdown of module requested...");
  mRunning = false;
  mNetworkSystemThread->join();
  TRACE_INFO("Shutdown of module done");
}

void NetworkSystem::addNewClientSocket(sf::TcpSocket* socket, int ID) {
  std::lock_guard<std::mutex> lockGuard(*mMapLock);
  mNewClientsSockets.emplace(ID, socket);
  TRACE_DEBUG("Added new client");
}

void NetworkSystem::removeClientSocket(int ID) {
  std::lock_guard<std::mutex> lockGuard(*mMapLock);
  mNewClientsSockets.erase(ID);
}

void NetworkSystem::updateInternalMap() {
  std::lock_guard<std::mutex> lockGuard(*mMapLock);
  mClientsSockets.clear();
  mClientsSockets.insert(mNewClientsSockets.begin(), mNewClientsSockets.end());
}