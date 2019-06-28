#include "NetworkSystem.h"

#include "../../../Common/Interfaces.h"
#include "../../../Common/MessageId.h"
#include "../../../Common/Messages/PlayerDataMessage.h"
#include "../../../Common/Messages/ChangeUsernameMessage.h"
#include "../../../Common/Messages/CharacterChoosenMessage.h"
#include "../../../Common/Messages/MapMessage.h"
#include "../../../Common/Messages/GameStateMessage.h"
#include "../../../Common/Messages/ServerReadyMessage.h"
#include "../../../Common/Messages/AddDebugButtonMessage.h"
#include "../../../Common/Messages/RemoveDebugButtonMessage.h"
#include "../../../Common/Messages/ServerInternal/ClientDisconnectedMessage.h"

#define HOST 1

NetworkSystem::NetworkSystem() {
  mName = "SERVER: NETWORK_SYSTEM";
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
  mNetworkSystemThread = std::make_unique<std::thread>(&NetworkSystem::run, this);
  TRACE_INFO("Starting module done");
}

void NetworkSystem::run() {
  mRunning = true;
  mNumberOfPlayerChanged = false;
  Interface inputListInterface;
  MessageHandler::get().publishInterface(Interfaces::SERVER_INPUT_LIST, &inputListInterface);
  MessageHandler::get().publishInterface(Interfaces::SERVER_SERVER_READY, &mServerServerReadyInterface);
  MessageHandler::get().publishInterface(Interfaces::SERVER_PLAYER_DATA, &mPlayerDataInterface);
  MessageHandler::get().publishInterface(Interfaces::SERVER_PLAYER_LOBBY, &mPlayerLobbyInterface);
  MessageHandler::get().publishInterface(Interfaces::SERVER_GAME_STATE, &mGameStateInterface);
  MessageHandler::get().publishInterface(Interfaces::SERVER_DEBUG_MENU, &mDebugMenuInterface);
  MessageHandler::get().publishInterface(Interfaces::SERVER_SOUND_LIST, &mSoundInterface);
  mClientsSockets.clear();
  mNewClientsSockets.clear();
  mDebugMenuInterface.getMessageQueue(); // Empty old queue.
  while (mRunning) {
    updateInternalMap();

    if (mHeartbeatTimer.getElapsedTime() > sf::milliseconds(1000)) {
      mHeartbeatTimer.restart();
      for (auto client : mClientsSockets) {
        if (client.second.second->getElapsedTime() > sf::milliseconds(30000)) {
          TRACE_WARNING("Received no heartbeat for 30 seconds. Lost connection to client!");

          ClientDisconnectedMessage cdm(client.first);
          mPlayerLobbyInterface.pushMessage(cdm.pack());

          removeClientSocket(client.first);
          mClientsSockets.erase(client.first);
          break;
        }

        sf::Packet packet;
        packet << MessageId::HEARTBEAT;
        sf::Socket::Status socketStatus = client.second.first->send(packet);
        if (socketStatus != sf::Socket::Done) {
          TRACE_WARNING("Could not send heartbeat to client. Lost connection to client!");

          ClientDisconnectedMessage cdm(client.first);
          mPlayerLobbyInterface.pushMessage(cdm.pack());

          removeClientSocket(client.first);
          mClientsSockets.erase(client.first);
          break;
        }
      }
    }

    for (auto client : mClientsSockets) {
      sf::Packet packet;
      if (client.second.first->receive(packet) == sf::Socket::Done) {
        int packetId = -1;
        packet >> packetId;
        switch (packetId) {
          case MessageId::INPUT_KEYS: {
            packet << client.first;
            inputListInterface.pushMessage(packet);
            break;
          }
          case MessageId::SHUT_DOWN:
            break;
          case MessageId::CHANGE_GAME_STATE: {
            // Check if it is the host
            if (client.first == HOST) {
              GameStateMessage gsm(packet);
              mGameStateInterface.pushMessage(gsm.pack());
            }
            break;
          }
          case MessageId::ADD_DEBUG_BUTTON: {
            AddDebugButtonMessage adbm(packet);
            mDebugMenuInterface.pushMessageTo(adbm.pack(), adbm.getSubscriberId());
            break;
          }
          case MessageId::NEW_USERNAME: {
            ChangeUsernameMessage cum(packet);
            cum.setId(client.first);
            mPlayerLobbyInterface.pushMessage(cum.pack());
            break;
          }
          case MessageId::CHARACTER_CHOOSEN: {
            CharacherChoosenMessage ccm(packet);
            ccm.setId(client.first);
            mPlayerLobbyInterface.pushMessage(ccm.pack());
            break;
          }
          case MessageId::MAP_DATA: {
            MapMessage mm(packet);
            mPlayerLobbyInterface.pushMessage(mm.pack());
          }
          case MessageId::HEARTBEAT: {
            client.second.second->restart();
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
        clientSocket.second.first->send(spriteDataPacket);
      }
    }

    handleServerReady();
    handlePlayerData();
    handleDebugMenu();
    handleSoundList();
    handleGameState();

    sf::sleep(sf::milliseconds(1));
  }

  TRACE_INFO("UNPUBLISHING");
  MessageHandler::get().unpublishInterface(Interfaces::SERVER_INPUT_LIST);
  MessageHandler::get().unpublishInterface(Interfaces::SERVER_SERVER_READY);
  MessageHandler::get().unpublishInterface(Interfaces::SERVER_PLAYER_DATA);
  MessageHandler::get().unpublishInterface(Interfaces::SERVER_PLAYER_LOBBY);
  MessageHandler::get().unpublishInterface(Interfaces::SERVER_GAME_STATE);
  MessageHandler::get().unpublishInterface(Interfaces::SERVER_DEBUG_MENU);
  MessageHandler::get().unpublishInterface(Interfaces::SERVER_SOUND_LIST);

  for (auto client : mClientsSockets) {
    client.second.first->disconnect();
    delete client.second.first;
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
  mNumberOfPlayerChanged = true;
  socket->setBlocking(false);
  mNewClientsSockets.emplace(ID, std::make_pair(socket, std::make_shared<sf::Clock>()));
  TRACE_DEBUG1("Added new client");
}

void NetworkSystem::removeClientSocket(int ID) {
  std::lock_guard<std::mutex> lockGuard(*mMapLock);
  mNumberOfPlayerChanged = true;
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
  if (mNumberOfPlayerChanged) {
    mClientsSockets.clear();
    mClientsSockets.insert(mNewClientsSockets.begin(), mNewClientsSockets.end());
    mNumberOfPlayerChanged = false;
  }
}

void NetworkSystem::handleServerReady() {
  if (mClientsSockets.empty()) {
    // We have no clients, no host, so dont check the server ready message since we cant send it.
    return;
  }

  auto serverReadyQueue = mServerServerReadyInterface.getMessageQueue();
  while (!serverReadyQueue.empty()) {
    auto serverReadyPacket = serverReadyQueue.front();
    serverReadyQueue.pop();
    int id = -1;
    serverReadyPacket >> id;

    if (id == MessageId::SERVER_READY) {
      ServerReadyMessage srm;
      for (auto clientSocket : mClientsSockets) {
        sf::Packet srmPacket = srm.pack();
        clientSocket.second.first->send(srmPacket);
      }
    } else {
      TRACE_WARNING("Received unhandled packet with id: " << id);
    }
  }
}

void NetworkSystem::handlePlayerData() {
  auto playerDataMessages = mPlayerDataInterface.getMessageQueue();
  while (playerDataMessages.size() > 0) {
    auto playerDataPacket = playerDataMessages.front();
    playerDataMessages.pop();

    int id = -1;
    playerDataPacket >> id;

    switch (id) {
      case MessageId::PLAYER_DATA: {
        PlayerDataMessage pdm(playerDataPacket);

        for (auto client : mClientsSockets) {
          sf::Packet packet = pdm.pack();
          client.second.first->send(packet);
        }
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
      case MessageId::ADD_DEBUG_BUTTON: {
        AddDebugButtonMessage adbm(debugMenuMessage);

        sf::Packet packet = adbm.pack();
        if (!mClientsSockets.empty()) {
          mClientsSockets.at(HOST).first->send(packet); // Send only to host, which is 1 TODO: Fix?
        }
        break;
      }
      case MessageId::REMOVE_DEBUG_BUTTON: {
        RemoveDebugButtonMessage rdbm(debugMenuMessage);
        sf::Packet packet = rdbm.pack();
        if (!mClientsSockets.empty()) {
          mClientsSockets.at(HOST).first->send(packet); // Send only to host, which is 1 TODO: Fix?
        }
        break;
      }
      default:
        TRACE_ERROR("Received unknown message: " << id);
        break;
      }
  }
}

void NetworkSystem::handleSoundList() {
  auto soundListQueue = mSoundInterface.getMessageQueue();
  while (!soundListQueue.empty()) {
    auto soundListPacket = soundListQueue.front();
    soundListQueue.pop();
    int id = -1;
    soundListPacket >> id;

    if (id == MessageId::SOUND_LIST) {
      SoundMessage sm(soundListPacket);
      for (auto clientSocket : mClientsSockets) {
        sf::Packet smPacket = sm.pack();
        clientSocket.second.first->send(smPacket);
      }
    } else {
      TRACE_WARNING("Received unhandled packet with id: " << id);
    }
  }
}

void NetworkSystem::handleGameState() {
  auto gameStateQueue = mGameStateInterface.getMessageQueue();
  while (!gameStateQueue.empty()) {
    auto gameStatePacket = gameStateQueue.front();
    gameStateQueue.pop();
    int id = -1;
    gameStatePacket >> id;

    if (id == MessageId::CHANGE_GAME_STATE) {
      GameStateMessage gsm(gameStatePacket);
      for (auto clientSocket : mClientsSockets) {
        sf::Packet gsmPacket = gsm.pack();
        clientSocket.second.first->send(gsmPacket);
      }
    } else {
      TRACE_WARNING("Received unhandled packet with id: " << id);
    }
  }
}