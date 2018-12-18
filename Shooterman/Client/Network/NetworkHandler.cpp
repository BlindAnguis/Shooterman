#include "NetworkHandler.h"

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
  mMessageSubscriber.setId(666);
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
    MessageHandler::get().pushGameStateMessage(gsm.pack());
    return;
  }

  TRACE_DEBUG("Unpacking message");

  IpMessage ipm(ipMessage);

  //std::string ip = sf::IpAddress::getLocalAddress().toString();
  TRACE_INFO("Connecting socket to " << ipm.getIp());
  sf::TcpSocket soc;
  auto connected = soc.connect(sf::IpAddress(ipm.getIp()), ipm.getPort());

  // Failed to connect to server
  if (connected != sf::Socket::Status::Done) {
    TRACE_INFO("Connection failed! " << connected);
    GameStateMessage gsm(GAME_STATE::MAIN_MENU);
    MessageHandler::get().pushGameStateMessage(gsm.pack());
    return;
  }

  TRACE_INFO("Connected!");
  PrivateCommunication pc;
  MessageHandler::get().publishInterface("ClientSpriteList", &pc);
  MessageHandler::get().subscribeTo("ClientInputList", &mMessageSubscriber);
  soc.setBlocking(false);
  mRunning = true;
  while (mRunning) {
    // 1. Receive new port for UDP from server
    // 2. Send port to clientMain
    // 3. Setup connection over UDP

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
        MessageHandler::get().pushSpriteListMessage(sm.pack());
      } else {
        TRACE_WARNING("Packet not known: " << id);
        break;
      }
    }
    sf::sleep(sf::milliseconds(1));
  }
  TRACE_INFO("Not running any more");
  soc.disconnect();
  MessageHandler::get().unpublishInterface("ClientSpriteList");
  MessageHandler::get().unsubscribeAll(&mMessageSubscriber);
}

void NetworkHandler::shutDown() {
  TRACE_INFO("Shutdown of module requested...");
  mRunning = false;
  mNetworkHandlerThread->join();
  TRACE_INFO("Shutdown of module done");
}
