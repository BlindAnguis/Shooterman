#pragma once

#include "gtest/gtest.h"

#include <memory>
#include <map>

#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>

#include "../Shooterman/Client/Network/NetworkHandler.cpp"
#include "../Shooterman/Common/MessageHandler/Mock/MessageHandlerMock.cpp"

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
  std::map<std::string, Subscriber*> mSubscriberMap;
  sf::TcpListener mServerSocket;
  sf::TcpSocket mClientSocket;

  void expectSubscribeTo(std::string interfaceName);
  void expectSubscribe(std::string interfaceName);
  void expectSubscribersToSubscribe();
  void expectInterfacesToPublish();
  void expectSubscribersToUnsubscribe();
  void expectInterfacesToUnpublish();

  void sendGameStateLobby(bool expectSubscribe = true);
  void sendGameStateJoin(bool expectSubscribe = true);
  void sendGameStateMainMenu(bool hasSocketBeenConnected = true);
  void sendIpMessage();
};

