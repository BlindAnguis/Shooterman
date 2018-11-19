#include "NetworkHandler.h"

#include <cstdint>

#include "../../Common/Trace.h"

NetworkHandler::NetworkHandler() {
  mName = "NETWORK_HANDLER";
  mDebugEnabled = true;
}

void NetworkHandler::start() {
  TRACE_INFO("Starting module...");
  mNetworkHandlerThread = std::make_unique<std::thread>(&NetworkHandler::startup, this);
  TRACE_INFO("Starting module done");
}

void NetworkHandler::startup() {
  // Setup a connection with clientMain
  PrivateCommunication nhToMainCommunication;
  MessageHandler::get().publishComm("nhToMainCommunication", &nhToMainCommunication);
  Subscriber subscriber;

  while (!MessageHandler::get().subscribeTo("mainToNhCommunication", &subscriber)) {
    sf::sleep(sf::milliseconds(1));
  }

  // Wait for IP and Port from clientMain
  auto m = subscriber.getMessageQueue();
  while (m.size() == 0) {
    m = subscriber.getMessageQueue();
  }
  SetupSocketConnectionData sscd;
  auto tmp = m.front();
  int id;
  tmp >> id; // TODO: Handle id
  sscd.unpack(m.front());
  std::string ip = sscd.getIP();
  unsigned short port = sscd.getPort();
  TRACE_INFO("Connecting socket to " << ip);
  bool connected = mTcp.connect(ip, port);

  // Failed to connect to server
  if (!connected) {
    GameStateMessage gsm(GAME_STATE::MAIN_MENU);
    MessageHandler::get().pushGameStateMessage(gsm.pack());
    MessageHandler::get().unsubscribeTo("nhToMainCommunication", &subscriber);
    MessageHandler::get().unpublishComm("nhToMainCommunication");
    return;
  }

  GameStateMessage gsm(GAME_STATE::PLAYING);
  MessageHandler::get().pushGameStateMessage(gsm.pack());
  MessageHandler::get().subscribeToSystemMessages(&mMessageSubscriber);
  MessageHandler::get().subscribeToGameStateMessages(&mMessageSubscriber);
  MessageHandler::get().subscribeToInputMessages(&mMessageSubscriber);

  while (mRunning) {
    // 1. Receive new port for UDP from server
    // 2. Send port to clientMain
    // 3. Setup connection over UDP

    TRACE_INFO("Listening to socket");
    std::queue<sf::Packet> systemMessageQueue = mMessageSubscriber.getMessageQueue();
    sf::Packet packet;
    while (!systemMessageQueue.empty()) {
      packet = systemMessageQueue.front();
      systemMessageQueue.pop();
      //mUdpSocket.send(packet);
    }

    readTcp();
    
    sf::sleep(sf::milliseconds(FRAME_LENGTH_IN_MS));
  }
  MessageHandler::get().unsubscribeAll(&mMessageSubscriber);
  MessageHandler::get().unpublishComm("nhToMainCommunication");
}

void NetworkHandler::shutDown() {
  TRACE_INFO("Shutdown of module requested...");
  mRunning = false;
  mNetworkHandlerThread->join();
  TRACE_INFO("Shutdown of module done");
}

void NetworkHandler::readTcp() {
  std::vector<sf::Packet> packets = mTcp.read();
  for (auto packet : packets) {
    int id = -1;
    packet >> id;
    if (id == SPRITE_LIST) {
      SpriteMessage sm;
      sm.unpack(packet);
      TRACE_DEBUG("Receveid sprite package");
      MessageHandler::get().pushSpriteListMessage(sm.pack());
    }
    else {
      TRACE_WARNING("Packet not known: " << id);
      break;
    }
  }
}

void NetworkHandler::readUdp() {
  std::vector<sf::Packet> packets = mUdp.read();
  for (auto packet : packets) {
    int id = -1;
    packet >> id;
    if (id == SPRITE_LIST) {
      SpriteMessage sm;
      sm.unpack(packet);
      TRACE_DEBUG("Receveid sprite package");
      MessageHandler::get().pushSpriteListMessage(sm.pack());
    }
    else {
      TRACE_WARNING("Packet not known: " << id);
      break;
    }
  }
}