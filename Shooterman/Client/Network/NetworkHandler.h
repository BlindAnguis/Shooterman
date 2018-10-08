#pragma once

#include <thread>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "../../Common/KeyBindings.h"
#include "../../Common/MessageId.h"
#include "../../Common/Constants.h"
#include "../MessageHandler/MessageHandler.h"
#include "../../Common/Messages/GameStateMessage.h"

class NetworkHandler : Trace {
public:
  NetworkHandler();

  void start();
  void shutDown();

private:
  bool mRunning = true;
  std::unique_ptr<std::thread> mNetworkHandlerThread;
  Subscriber mMessageSubscriber;
  sf::TcpSocket mSocket;

  void startup();
};