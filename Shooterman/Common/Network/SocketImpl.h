#pragma once

#include "Socket.h"

#include <SFML/Network/TcpSocket.hpp>

class SocketImpl : public Socket {
public:
  SocketImpl();
  ~SocketImpl();

  Soc::Status connect(std::string ipAddress, std::uint16_t port, int timeout) override;
  void disconnect() override;

  void setBlocking(bool blocking) override;

  void send(sf::Packet& packet) override;
  Soc::Status receive(sf::Packet& packet)  override;

private:
  sf::TcpSocket mSocket;
};

