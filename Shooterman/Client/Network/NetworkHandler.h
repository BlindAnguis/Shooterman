#pragma once

#include <thread>

#include <SFML/Network.hpp>

#include "../../Common/MessageHandler/Interface.h"
#include "../../Common/Trace.h"

class NetworkHandler : Trace {
public:
  NetworkHandler();

  void start();
  void shutDown();

private:
  bool mRunning = true;
  std::unique_ptr<std::thread> mNetworkHandlerThread;
  Interface mLobbyInterface;
  Interface mSpriteListInterface;
  Interface mPlayerDataInterface;
  Subscriber mMessageSubscriber;
  Subscriber mDebugSubscriber;
  Subscriber mGameStateSubscriber;
  Subscriber mServerDebugSubscriber;
  sf::TcpSocket mSocket;

  void startup();
  void setupSubscribersAndInterfaces();
  sf::Socket::Status setupSocketConnection();
  void handlePackets();
  void teardownSubscribersAndInterfaces();

  void handleDebugMessages();
};