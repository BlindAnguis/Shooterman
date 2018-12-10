#pragma once

#include <string>

#include "Socket.h"

class UdpSocket : public Socket {
public:
  UdpSocket(std::string ip, unsigned short port);
  UdpSocket() {}
  ~UdpSocket();

  bool connect() override;

  void disconnect() override;

  std::vector<sf::Packet> read() override;

  void send(sf::Packet packet) override;

private:
  sf::UdpSocket* mUdpSocket;

};