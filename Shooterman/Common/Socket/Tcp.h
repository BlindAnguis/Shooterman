#pragma once

#include <string>

#include "Socket.h"

class TcpSocket : public Socket {
public:
  TcpSocket();

  bool connect(std::string ip, unsigned short port) override;

  void disconnect() override;

  std::vector<sf::Packet> read() override;

  void send(sf::Packet packet) override;

private:
  sf::TcpSocket mTcpSocket;
};