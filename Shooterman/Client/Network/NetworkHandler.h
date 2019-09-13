#pragma once

#include <thread>

#include <SFML/Network/TcpSocket.hpp>
#include <SFML/System/Clock.hpp>

#include "../../Common/MessageHandler/Interface.h"
#include "../../Common/MessageHandler/MessageHandler.h"
#include "../../Common/Trace.h"

#include "../../Common/Process/Runnable.h"

enum STATE { Disconnected, Connecting, Connected, Disconnecting };

class NetworkHandler : public Runnable, Trace {
public:
  NetworkHandler(std::shared_ptr<MessageHandler> messageHandler);
  ~NetworkHandler();

  void start() override;
  void run() override;
  void stop() override;

private:
  bool mSubscribedToIpMessage = false;
  int mConnectionTriesLeft;
  std::unique_ptr<std::thread> mNetworkHandlerThread;
  Interface mLobbyInterface;
  Interface mSpriteListInterface;
  Interface mPlayerDataInterface;
  Interface mSoundListInterface;
  Interface mServerReadyInterface;
  Subscriber mIpSubscriber;
  Subscriber mForwaringSubscriber;
  Subscriber mInfoMessageSubscriber;
  Subscriber mGameStateSubscriber;
  Subscriber mServerDebugSubscriber;
  std::shared_ptr<MessageHandler> mMessageHandler;
  sf::TcpSocket mSocket;
  sf::Clock mHeartbeatClock;
  STATE mCurrentState;
  std::string mServerIp;
  unsigned short mServerPort;

  void setupSubscribersAndInterfaces();
  void handlePackets();
  void teardownSubscribersAndInterfaces();

  void handleSubscribeIpListTimeoutMessage(sf::Packet& message);
  void handleIpListMessage(sf::Packet& message);
  void handleSubscribeGameStateTimeoutMessage(sf::Packet& message);
  void handleChangeGameStateMessage(sf::Packet& message);
};