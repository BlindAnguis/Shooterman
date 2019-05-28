#include "NetworkSystem.h"

#include "../../../Common/MessageId.h"
#include "../../../Common/Messages/PlayerDataMessage.h"
#include "../../../Common/Messages/GameStateMessage.h"
#include "../../../Common/Messages/AddDebugButtonMessage.h"
#include "../../../Common/Messages/RemoveDebugButtonMessage.h"

NetworkSystem::NetworkSystem() {
  mName = "SERVER: NETWORK_SYSTEM";
  mDebugEnabled1 = true;
  mMapLock = new std::mutex();
  mRenderLock = new std::mutex();
}

NetworkSystem::~NetworkSystem() {
  delete mMapLock;
  delete mRenderLock;
  TRACE_DEBUG1("Enter Destructor");
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
  MessageHandler::get().publishInterface("ServerDebugMenu", &mDebugMenuInterface);
  MessageHandler::get().publishInterface("ServerSoundList", &mSoundInterface);
  mClientsSockets.clear();
  mNewClientsSockets.clear();
  mDebugMenuInterface.getMessageQueue(); // Empty old queue.
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
          break;
        }
        case ADD_DEBUG_BUTTON:
        {
          AddDebugButtonMessage adbm(packet);
          mDebugMenuInterface.pushMessageTo(adbm.pack(), adbm.getSubscriberId());
          break;
        }
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
        TRACE_WARNING("Received unhandled packet with id: " << id);
      }
    }

    auto soundListQueue = mSoundInterface.getMessageQueue();
    while (!soundListQueue.empty()) {
      auto soundListPacket = soundListQueue.front();
      soundListQueue.pop();
      int id = -1;
      soundListPacket >> id;

      if (id == SOUND_LIST) {
        SoundMessage sm;
        sm.unpack(soundListPacket);
        for (auto clientSocket : mClientsSockets) {
          sf::Packet smPacket = sm.pack();
          clientSocket.second->send(smPacket);
        }
      }
      else {
        TRACE_WARNING("Received unhandled packet with id: " << id);
      }
    }

    handlePlayerData();
    handleDebugMenu();

    sf::sleep(sf::milliseconds(1));
  }

  TRACE_INFO("UNPUBLISHING");
  MessageHandler::get().unpublishInterface("ServerInputList");
  MessageHandler::get().unpublishInterface("ServerPlayerData");
  MessageHandler::get().unpublishInterface("ServerGameState");
  MessageHandler::get().unpublishInterface("ServerDebugMenu");
  MessageHandler::get().unpublishInterface("ServerSoundList");
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
  TRACE_DEBUG1("Added new client");
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

void NetworkSystem::handleDebugMenu() {
  auto debugMenuQueue = mDebugMenuInterface.getMessageQueue();
  while (!debugMenuQueue.empty()) {
    sf::Packet debugMenuMessage = debugMenuQueue.front();
    debugMenuQueue.pop();

    int id = -1;
    debugMenuMessage >> id;

    switch (id) {
    case ADD_DEBUG_BUTTON: {
      AddDebugButtonMessage adbm(debugMenuMessage);

      sf::Packet packet = adbm.pack();
      if (!mClientsSockets.empty()) {
        mClientsSockets.at(2)->send(packet); // Send only to host, which is 2 TODO: Fix?
      }
      break;
    }
    case REMOVE_DEBUG_BUTTON:
    {
      RemoveDebugButtonMessage rdbm(debugMenuMessage);
      sf::Packet packet = rdbm.pack();
      if (!mClientsSockets.empty()) {
        mClientsSockets.at(2)->send(packet); // Send only to host, which is 2 TODO: Fix?
      }
      break;
    }
    default:
      TRACE_ERROR("Received unknown message: " << id);
      break;
    }
  }
}