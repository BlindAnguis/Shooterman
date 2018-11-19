#pragma once

#include <thread>

#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "../MessageHandler/MessageHandler.h"
#include "../../Common/KeyBindings.h"
#include "../../Common/MessageId.h"
#include "../../Common/Constants.h"
#include "../../Common/Messages/GameStateMessage.h"
#include "../../Common/Messages/SpriteMessage.h"
#include "../../Common/Messages/ClientMainAndNetworkHandlerMessages.h"
#include "../../Common/Socket/Tcp.h"
#include "../../Common/Socket/Udp.h"

class NetworkHandler : Trace {
public:
  NetworkHandler();

  void start();
  void shutDown();

private:
  bool mRunning = true;
  std::unique_ptr<std::thread> mNetworkHandlerThread;
  Subscriber mMessageSubscriber;
  UdpSocket mUdp;
  TcpSocket mTcp;

  void startup();

  void readTcp();
  void readUdp();

};