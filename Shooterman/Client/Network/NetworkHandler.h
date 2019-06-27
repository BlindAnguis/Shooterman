#pragma once

#include <thread>

#include <SFML/Network/TcpSocket.hpp>

#include "../../Common/MessageHandler/Interface.h"
#include "../../Common/Trace.h"

enum CONNECTION_STATUS { Done, NotReady, Partial, Disconnected, Error, Cancel };

class NetworkHandler : Trace {
public:
  NetworkHandler();
  ~NetworkHandler();

  void start();
  void shutDown();

private:
  bool mRunning = true;
  std::unique_ptr<std::thread> mNetworkHandlerThread;
  Interface mLobbyInterface;
  Interface mSpriteListInterface;
  Interface mPlayerDataInterface;
  Interface mSoundListInterface;
  Interface mServerReadyInterface;
  Subscriber mInfoMessageSubscriber;
  Subscriber mMessageSubscriber;
  Subscriber mGameStateSubscriber;
  Subscriber mServerDebugSubscriber;
  sf::TcpSocket mSocket;
  sf::Clock mHeartbeatClock;

  void run();
  void setupSubscribersAndInterfaces();
  CONNECTION_STATUS setupSocketConnection();
  bool checkIfConnectionIsCancelled();
  void handlePackets();
  void teardownSubscribersAndInterfaces();
};