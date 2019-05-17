#include "NetworkSystem.h"

#include "../../../Common/MessageId.h"
#include "../../../Common/Messages/PlayerDataMessage.h"
#include "../../../Common/Messages/GameStateMessage.h"

NetworkSystem::NetworkSystem() {
  mName = "SERVER: NETWORK_SYSTEM";
  mDebugEnabled = true;
  mMapLock = new std::mutex();
  mRenderLock = new std::mutex();
}

NetworkSystem::~NetworkSystem() {
  delete mMapLock;
  delete mRenderLock;
  TRACE_DEBUG("Enter Destructor");
}

NetworkSystem& NetworkSystem::get() {
  static NetworkSystem instance;
  return instance;
}

void NetworkSystem::start() {
  TRACE_INFO("Starting module...");
  mNetworkSystemThread = std::make_unique<std::thread>(&NetworkSystem::startup, this);
  TRACE_INFO("Starting module done");
}

void NetworkSystem::startup() {
  mRunning = true;
  Interface inputListInterface;
  MessageHandler::get().publishInterface("ServerInputList", &inputListInterface);
  MessageHandler::get().publishInterface("ServerPlayerData", &mPlayerDataInterface);
  MessageHandler::get().publishInterface("ServerGameState", &mGameStateInterface);
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
          inputListInterface.pushMessage(packet);
          break;
        }
        case SHUT_DOWN:
          break;
        case CHANGE_GAME_STATE:
        {
          // Check if it is the host
          if (client.first == 2) {
            GameStateMessage gsm;
            gsm.unpack(packet);
            mGameStateInterface.pushMessage(gsm.pack());
          }
        }
          break;
        default:
          TRACE_ERROR("Received unhandled packet with ID: " << packetId);
          break;
        }
      }
    }

    auto spriteData = getRenderData();
    if (spriteData) {
      for (auto clientSocket : mClientsSockets) {
        sf::Packet spriteDataPacket = spriteData->pack();
        clientSocket.second->send(spriteDataPacket);
      }
    }

    auto gameStateQueue = mGameStateInterface.getMessageQueue();
    while (!gameStateQueue.empty()) {
      auto gameStatePacket = gameStateQueue.front();
      gameStateQueue.pop();
      int id = -1;
      gameStatePacket >> id;
      
      if (id == CHANGE_GAME_STATE) {
        GameStateMessage gsm;
        gsm.unpack(gameStatePacket);
        for (auto clientSocket : mClientsSockets) {
          sf::Packet gsmPacket = gsm.pack();
          clientSocket.second->send(gsmPacket);
        }
      } else {
        TRACE_WARNING("Received unhandeled packet with id: " << id);
      }
    }

    handlePlayerData();

    sf::sleep(sf::milliseconds(1));
  }

  TRACE_INFO("UNPUBLISHING");
  MessageHandler::get().unpublishInterface("ServerInputList");
  MessageHandler::get().unpublishInterface("ServerPlayerData");
  MessageHandler::get().unpublishInterface("ServerGameState");
  for (auto client : mClientsSockets) {
    client.second->disconnect();
    delete client.second;
  }
}

void NetworkSystem::shutDown() {
  TRACE_INFO("Shutdown of module requested...");
  if (mRunning) {
    mRunning = false;
    mNetworkSystemThread->join();
  }
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

void NetworkSystem::setRenderData(std::shared_ptr<SpriteMessage> spriteMessage) {
  std::lock_guard<std::mutex> lockGuard(*mRenderLock);
  mSpriteMessage = spriteMessage;
}

std::shared_ptr<SpriteMessage> NetworkSystem::getRenderData() {
  std::lock_guard<std::mutex> lockGuard(*mRenderLock);
  auto spriteData = mSpriteMessage;
  mSpriteMessage = nullptr;
  return spriteData;
}

void NetworkSystem::updateInternalMap() {
  std::lock_guard<std::mutex> lockGuard(*mMapLock);
  mClientsSockets.clear();
  mClientsSockets.insert(mNewClientsSockets.begin(), mNewClientsSockets.end());
}

void NetworkSystem::handlePlayerData() {
  auto playerDataMessages = mPlayerDataInterface.getMessageQueue();
  while (playerDataMessages.size() > 0) {
    auto playerDataPacket = playerDataMessages.front();
    playerDataMessages.pop();

    int id = -1;
    playerDataPacket >> id;

    switch (id) {
    case PLAYER_DATA:
    {
      PlayerDataMessage pdm;
      pdm.unpack(playerDataPacket);
      int playerId = pdm.getPlayerId();

      sf::Packet packet = pdm.pack();
      mClientsSockets.at(playerId)->send(packet);
    }
      break;
    default:
      TRACE_ERROR("Received unknown message: " << id);
      break;
    }
  }
}