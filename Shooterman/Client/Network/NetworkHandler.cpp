#include "NetworkHandler.h"

#include "../../Common/Messages/PlayerDataMessage.h"

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
  while (!MessageHandler::get().subscribeTo("ClientDebugMenu", &mDebugSubscriber)) {
    sf::sleep(sf::milliseconds(5));
  }  
  Subscriber gameStateSubscriber;
  MessageHandler::get().subscribeTo("GameState", &gameStateSubscriber);
  AddDebugButtonMessage debMess(mDebugSubscriber.getId(), "Client network debug traces");
  mDebugSubscriber.reverseSendMessage(debMess.pack());
  TRACE_DEBUG("Trying to subscribe to ClientIpList");
  while (!MessageHandler::get().subscribeTo("ClientIpList", &mMessageSubscriber)) {
    sf::sleep(sf::milliseconds(5));
  }
  TRACE_DEBUG("Subscribed to ClientIpList");

  std::queue<sf::Packet> messages;
  while (messages.size() == 0) {
    messages = mMessageSubscriber.getMessageQueue();
    sf::sleep(sf::milliseconds(5));
  }
  TRACE_DEBUG("Got IP message");

  MessageHandler::get().unsubscribeTo("ClientIpList", &mMessageSubscriber);
  TRACE_DEBUG("Unsubscribed to ClientIpList");

  int ID = -1;
  auto ipMessage = messages.front();
  ipMessage >> ID;
  if (ID != IP_MESSAGE) {
    TRACE_ERROR("Received unexpected message with ID: " << ID);
    GameStateMessage gsm(GAME_STATE::MAIN_MENU);
	  gameStateSubscriber.reverseSendMessage(gsm.pack());
    MessageHandler::get().unsubscribeTo("GameState", &gameStateSubscriber);
    return;
  }

  TRACE_DEBUG("Unpacking message");

  IpMessage ipm(ipMessage);

  TRACE_INFO("Connecting socket to " << ipm.getIp());
  sf::TcpSocket soc;
  auto connected = soc.connect(sf::IpAddress(ipm.getIp()), ipm.getPort(), sf::milliseconds(10000));

  // Failed to connect to server
  if (connected != sf::Socket::Status::Done) {
    TRACE_INFO("Connection failed! " << connected);
    GameStateMessage gsm(GAME_STATE::MAIN_MENU);
	  gameStateSubscriber.reverseSendMessage(gsm.pack());
    MessageHandler::get().unsubscribeTo("GameState", &gameStateSubscriber);
    return;
  }

  TRACE_INFO("Connected!");
  Interface spriteListInterface;
  MessageHandler::get().publishInterface("ClientSpriteList", &spriteListInterface);
  Interface playerDataInterface;
  MessageHandler::get().publishInterface("ClientPlayerData", &playerDataInterface);
  MessageHandler::get().subscribeTo("ClientInputList", &mMessageSubscriber);
  soc.setBlocking(false);
  mRunning = true;
  while (mRunning) {
    std::queue<sf::Packet> systemMessageQueue = mMessageSubscriber.getMessageQueue();
    while (!systemMessageQueue.empty()) {
      sf::Packet packet = systemMessageQueue.front();
      systemMessageQueue.pop();
      soc.send(packet);
      //TRACE_DEBUG("SENDING MESAGE");
    }

    sf::Packet packet;
    if (soc.receive(packet) == sf::Socket::Done) {
      int id = -1;
      packet >> id;
      if (id == SPRITE_LIST) {
        SpriteMessage sm;
        sm.unpack(packet);
        //TRACE_DEBUG("Receveid sprite package with id: " << id);
        spriteListInterface.pushMessage(sm.pack());
      } else if (id == SPRITE_LIST_CACHE) {
        SpriteCacheMessage scm;
        scm.unpack(packet);
        spriteListInterface.pushMessage(scm.pack());
      } else if (id == PLAYER_DATA) {
        PlayerDataMessage pdm;
        pdm.unpack(packet);
        playerDataInterface.pushMessage(pdm.pack());
      } else {
        TRACE_ERROR("Packet not known: " << id);
      }
    }
    sf::sleep(sf::milliseconds(1));
  }
  TRACE_INFO("Not running any more");
  soc.disconnect();
  MessageHandler::get().unpublishInterface("ClientSpriteList");
  MessageHandler::get().unsubscribeTo("ClientInputList", &mMessageSubscriber);
  MessageHandler::get().unsubscribeTo("ClientDebugMenu", &mDebugSubscriber);
  MessageHandler::get().unsubscribeTo("GameState", &gameStateSubscriber);
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