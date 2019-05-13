#pragma once

#include <thread>

#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "../../Common/MessageHandler/Interface.h"
#include "../../Common/Trace.h"
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
  Interface mLobbyInterface;
  Subscriber mMessageSubscriber;
  Subscriber mDebugSubscriber;
  UdpSocket mUdp;
  TcpSocket mTcp;

  void startup();
  void handleDebugMessages();
};