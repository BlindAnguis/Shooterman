#pragma once

#include "gtest/gtest.h"

#include <memory>
#include <map>

#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>

#include "../Shooterman/Common/MessageHandler/Mock/MessageHandlerMock.cpp"

#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING  1 // Remove stupid google test warning, or something


class NetworkHandlerTest : public ::testing::Test {
public:
  NetworkHandlerTest();
  ~NetworkHandlerTest();

  void SetUp();
  void TearDown();

  void expectSubscribeTo(std::string interfaceName);
  void expectSubscribe(std::string interfaceName);
  void expectSubscribersToSubscribe();
  void expectInterfacesToPublish();
  void expectSubscribersToUnsubscribe();
  void expectInterfacesToUnpublish();

protected:
  std::shared_ptr<MessageHandlerMock> mMessageHandlerMock;
  std::map<std::string, Subscriber*> mSubscriberMap;
  sf::TcpListener mServerSocket;
  sf::TcpSocket mClientSocket;
};

