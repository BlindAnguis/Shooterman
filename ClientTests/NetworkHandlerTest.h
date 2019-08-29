#pragma once

#include "gtest/gtest.h"

#include <memory>
#include <map>

#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>

#include "../Shooterman/Client/Network/NetworkHandler.cpp"
#include "../Shooterman/Common/MessageHandler/Mock/MessageHandlerMock.cpp"
#include "../Shooterman/Common/Network/Mock/SocketMock.cpp"

#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING  1 // Remove stupid google test warning, or something

class NetworkHandlerTest : public ::testing::Test {
public:
  NetworkHandlerTest();
  ~NetworkHandlerTest();

  void SetUp();
  void TearDown();

protected:
  std::shared_ptr<NetworkHandler> mNetworkHandler;
  std::shared_ptr<MessageHandlerMock> mMessageHandlerMock;
  std::shared_ptr<SocketMock> mSocketMock;
  std::map<std::string, Subscriber*> mSubscriberMap;

  void expectSubscribeTo(std::string interfaceName);
  void expectSubscribe(std::string interfaceName);
  void expectSubscribersToSubscribe();
  void expectInterfacesToPublish();
  void expectSubscribersToUnsubscribe();
  void expectInterfacesToUnpublish();

  void sendGameStateLobby(bool expectSubscribe = true);
  void sendGameStateJoin(bool expectSubscribe = true);
  void sendGameStateMainMenu(bool hasSocketBeenConnected = true);
  void sendIpMessage(Soc::Status connectionResult = Soc::Status::Done);
};

