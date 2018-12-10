#pragma once

#include <string>

#include "Socket.h"

class TcpSocket : public Socket {
public:
  TcpSocket(std::string ip, unsigned short port);
  TcpSocket(sf::TcpSocket* socket);
  TcpSocket() {}

  ~TcpSocket();

  bool connect() override;

  void disconnect() override;

  std::vector<sf::Packet> read() override;

  void send(sf::Packet packet) override;

private:
  sf::TcpSocket* mTcpSocket;
};