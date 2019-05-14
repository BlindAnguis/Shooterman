#include "NetworkHandler.h"

#include "../../Common/KeyBindings.h"
#include "../../Common/Constants.h"
#include "../../Common/MessageId.h"
#include "../../Common/MessageHandler/MessageHandler.h"
#include "../../Common/Messages/AddDebugButtonMessage.h"
#include "../../Common/Messages/ClientMainAndNetworkHandlerMessages.h"
#include "../../Common/Messages/GameStateMessage.h"
#include "../../Common/Messages/LobbyDataMessage.h"
#include "../../Common/Messages/PlayerDataMessage.h"
#include "../../Common/Messages/SpriteMessage.h"
#include "../../Common/Messages/SpriteCacheMessage.h"
#include "../../Common/Messages/ClientInternal/IpMessage.h"

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
  setupSubscribersAndInterfaces();

  sf::Socket::Status connectionStatus = setupSocketConnection();

  // Failed to connect to server
  if (connectionStatus != sf::Socket::Status::Done) {
    TRACE_INFO("Connection failed! " << connectionStatus);
    GameStateMessage gsm(GAME_STATE::MAIN_MENU);
    mGameStateSubscriber.reverseSendMessage(gsm.pack());
    MessageHandler::get().unsubscribeTo("GameState", &mGameStateSubscriber);
    return;
  }

  TRACE_INFO("Connected!");

  MessageHandler::get().publishInterface("ClientSpriteList", &mSpriteListInterface);
  MessageHandler::get().publishInterface("ClientPlayerData", &mPlayerDataInterface);
  MessageHandler::get().subscribeTo("ClientInputList", &mMessageSubscriber);

  mSocket.setBlocking(false);
  mRunning = true;

  handlePackets();
 
  TRACE_INFO("Not running any more");
  mSocket.disconnect();
  teardownSubscribersAndInterfaces();
}

void NetworkHandler::setupSubscribersAndInterfaces() {
  MessageHandler::get().publishInterface("ClientLobby", &mLobbyInterface);

  while (!MessageHandler::get().subscribeTo("ClientDebugMenu", &mDebugSubscriber)) {
    sf::sleep(sf::milliseconds(5));
  }
  AddDebugButtonMessage debMess(mDebugSubscriber.getId(), "Client network debug traces");
  mDebugSubscriber.reverseSendMessage(debMess.pack());

  while (!MessageHandler::get().subscribeTo("GameState", &mGameStateSubscriber)) {
    sf::sleep(sf::milliseconds(5));
  }

  while (!MessageHandler::get().subscribeTo("ClientIpList", &mMessageSubscriber)) {
    sf::sleep(sf::milliseconds(5));
  }
}

sf::Socket::Status NetworkHandler::setupSocketConnection() {
  sf::Socket::Status connectionStatus = sf::Socket::Status::Disconnected;

  // Wait for IP address
  std::queue<sf::Packet> messages;
  while (messages.size() == 0) {
    messages = mMessageSubscriber.getMessageQueue();
    sf::sleep(sf::milliseconds(5));
  }
  // We have the IP, no need to be subscribed
  MessageHandler::get().unsubscribeTo("ClientIpList", &mMessageSubscriber);

  int ID = -1;
  auto ipMessage = messages.front();
  ipMessage >> ID;
  if (ID != IP_MESSAGE) {
    // Received unexpected message
    TRACE_ERROR("Received unexpected message with ID: " << ID);
    GameStateMessage gsm(GAME_STATE::MAIN_MENU);
    mGameStateSubscriber.reverseSendMessage(gsm.pack());
    MessageHandler::get().unsubscribeTo("GameState", &mGameStateSubscriber);
    return connectionStatus;
  }

  IpMessage ipm(ipMessage);
  TRACE_INFO("Connecting socket to " << ipm.getIp());
  int connectionAttempts = 10;
  // Try to connect multiple times
  while (connectionStatus != sf::Socket::Status::Done && connectionAttempts > 0) {
    connectionStatus = mSocket.connect(sf::IpAddress(ipm.getIp()), ipm.getPort(), sf::milliseconds(100));
    connectionAttempts--;
    sf::sleep(sf::milliseconds(500));
  }

  return connectionStatus;
}

void NetworkHandler::handlePackets() {
  while (mRunning) {
    std::queue<sf::Packet> systemMessageQueue = mMessageSubscriber.getMessageQueue();
    while (!systemMessageQueue.empty()) {
      sf::Packet packet = systemMessageQueue.front();
      systemMessageQueue.pop();
      mSocket.send(packet);
    }

    sf::Packet packet;
    if (mSocket.receive(packet) == sf::Socket::Done) {
      int id = -1;
      packet >> id;
      if (id == SPRITE_LIST_CACHE) {
        SpriteCacheMessage sm;
        sm.unpack(packet);
        mSpriteListInterface.pushMessage(sm.pack());
      } else if (id == SPRITE_LIST) {
        SpriteMessage sm;
        sm.unpack(packet);
        mSpriteListInterface.pushMessage(sm.pack());
      } else if (id == PLAYER_DATA) {
        PlayerDataMessage pdm;
        pdm.unpack(packet);
        mPlayerDataInterface.pushMessage(pdm.pack());
      } else if (id == LOBBY_DATA) {
        LobbyDataMessage ldm;
        ldm.unpack(packet);
        mLobbyInterface.pushMessage(ldm.pack());
      } else {
        TRACE_ERROR("Packet not known: " << id);
      }
    }
    sf::sleep(sf::milliseconds(1));
  }
}

void NetworkHandler::teardownSubscribersAndInterfaces() {
  MessageHandler::get().unpublishInterface("ClientSpriteList");
  MessageHandler::get().unpublishInterface("ClientLobby");
  MessageHandler::get().unpublishInterface("ClientPlayerData");
  MessageHandler::get().unsubscribeTo("ClientInputList", &mMessageSubscriber);
  MessageHandler::get().unsubscribeTo("ClientDebugMenu", &mDebugSubscriber);
  MessageHandler::get().unsubscribeTo("GameState", &mGameStateSubscriber);
}

void NetworkHandler::shutDown() {
  TRACE_INFO("Shutdown of module requested...");
  mRunning = false;
  mNetworkHandlerThread->join();
  TRACE_INFO("Shutdown of module done");
}

void NetworkHandler::handleDebugMessages() {
  std::queue<sf::Packet> debugMessageQueue = mDebugSubscriber.getMessageQueue();
  sf::Packet debugMessage;
  while (!debugMessageQueue.empty()) {
    debugMessage = debugMessageQueue.front();
    debugMessageQueue.pop();
    mDebugEnabled = !mDebugEnabled;
  }
}