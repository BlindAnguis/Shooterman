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

NetworkSystem::NetworkSystem(std::shared_ptr<MessageHandler> messageHandler) : mMessageHandler(messageHandler) {
  mName = "SERVER: NETWORK_SYSTEM";
  mMapLock = new std::mutex();
  mRenderLock = new std::mutex();
}

NetworkSystem::~NetworkSystem() {
  delete mMapLock;
  delete mRenderLock;
  TRACE_DEBUG1("Enter Destructor");
}

void NetworkSystem::start() {
  TRACE_INFO("Starting module...");
  mNetworkSystemThread = std::make_unique<std::thread>(&NetworkSystem::run, this);
  TRACE_INFO("Starting module done");
}

void NetworkSystem::run() {
  mRunning = true;
  mNumberOfPlayerChanged = false;
  mServerServerReadyInterface.addSignalCallback(MessageId::SERVER_READY, std::bind(&NetworkSystem::handleServerReadyMessage, this, std::placeholders::_1));
  mPlayerDataInterface.addSignalCallback(MessageId::PLAYER_DATA, std::bind(&NetworkSystem::handlePlayerDataMessage, this, std::placeholders::_1));
  mDebugMenuInterface.addSignalCallback(MessageId::ADD_DEBUG_BUTTON, std::bind(&NetworkSystem::handleAddDebugButtonMessage, this, std::placeholders::_1));
  mDebugMenuInterface.addSignalCallback(MessageId::REMOVE_DEBUG_BUTTON, std::bind(&NetworkSystem::handleRemoveDebugButtonMessage, this, std::placeholders::_1));
  mSoundInterface.addSignalCallback(MessageId::SOUND_LIST, std::bind(&NetworkSystem::handleSoundListMessage, this, std::placeholders::_1));
  mGameStateInterface.addSignalCallback(MessageId::CHANGE_GAME_STATE, std::bind(&NetworkSystem::handleChangeGameStateMessage, this, std::placeholders::_1));

  Interface inputListInterface;
  mMessageHandler->publishInterface(Interfaces::SERVER_INPUT_LIST, &inputListInterface);
  mMessageHandler->publishInterface(Interfaces::SERVER_SERVER_READY, &mServerServerReadyInterface);
  mMessageHandler->publishInterface(Interfaces::SERVER_PLAYER_DATA, &mPlayerDataInterface);
  mMessageHandler->publishInterface(Interfaces::SERVER_PLAYER_LOBBY, &mPlayerLobbyInterface);
  mMessageHandler->publishInterface(Interfaces::SERVER_GAME_STATE, &mGameStateInterface);
  mMessageHandler->publishInterface(Interfaces::SERVER_DEBUG_MENU, &mDebugMenuInterface);
  mMessageHandler->publishInterface(Interfaces::SERVER_SOUND_LIST, &mSoundInterface);
  mClientsSockets.clear();
  mNewClientsSockets.clear();
  mDebugMenuInterface.clearMessages();
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
            CharacterChoosenMessage ccm(packet);
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

    if (!mClientsSockets.empty()) {
      // We have clients and/or host, so check the server ready message since we can send it.
      mServerServerReadyInterface.handleMessages();
    }
    mPlayerDataInterface.handleMessages();
    mDebugMenuInterface.handleMessages();
    mSoundInterface.handleMessages();
    mGameStateInterface.handleMessages();
    //handleServerReady();
    //handlePlayerData();
    //handleDebugMenu();
    //handleSoundList();
    //handleGameState();

    sf::sleep(sf::milliseconds(1));
  }

  TRACE_INFO("UNPUBLISHING");
  mMessageHandler->unpublishInterface(Interfaces::SERVER_INPUT_LIST);
  mMessageHandler->unpublishInterface(Interfaces::SERVER_SERVER_READY);
  mMessageHandler->unpublishInterface(Interfaces::SERVER_PLAYER_DATA);
  mMessageHandler->unpublishInterface(Interfaces::SERVER_PLAYER_LOBBY);
  mMessageHandler->unpublishInterface(Interfaces::SERVER_GAME_STATE);
  mMessageHandler->unpublishInterface(Interfaces::SERVER_DEBUG_MENU);
  mMessageHandler->unpublishInterface(Interfaces::SERVER_SOUND_LIST);

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

void NetworkSystem::handleServerReadyMessage(sf::Packet& message) {
  ServerReadyMessage srm;

  for (auto clientSocket : mClientsSockets) {
    sf::Packet srmPacket = srm.pack();
    clientSocket.second.first->send(srmPacket);
  }
}

void NetworkSystem::handlePlayerDataMessage(sf::Packet& message) {
  PlayerDataMessage pdm(message);

  for (auto client : mClientsSockets) {
    sf::Packet packet = pdm.pack();
    client.second.first->send(packet);
  }
}

void NetworkSystem::handleAddDebugButtonMessage(sf::Packet& message) {
  AddDebugButtonMessage adbm(message);

  sf::Packet packet = adbm.pack();
  if (!mClientsSockets.empty()) {
    mClientsSockets.at(HOST).first->send(packet); // Send only to host, which is 1 TODO: Fix?
  }
}

void NetworkSystem::handleRemoveDebugButtonMessage(sf::Packet& message) {
  RemoveDebugButtonMessage rdbm(message);

  sf::Packet packet = rdbm.pack();
  if (!mClientsSockets.empty()) {
    mClientsSockets.at(HOST).first->send(packet); // Send only to host, which is 1 TODO: Fix?
  }
}

void NetworkSystem::handleSoundListMessage(sf::Packet& message) {
  SoundMessage sm(message);

  for (auto clientSocket : mClientsSockets) {
    sf::Packet smPacket = sm.pack();
    clientSocket.second.first->send(smPacket);
  }
}

void NetworkSystem::handleChangeGameStateMessage(sf::Packet& message) {
  GameStateMessage gsm(message);
  for (auto clientSocket : mClientsSockets) {
    sf::Packet gsmPacket = gsm.pack();
    clientSocket.second.first->send(gsmPacket);
  }
}
