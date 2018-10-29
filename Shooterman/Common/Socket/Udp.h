#pragma once

#include <string>

#include "Socket.h"

class UdpSocket : public Socket {
public:
  UdpSocket();

  bool connect(std::string ip, unsigned short port) override;

  void disconnect() override;

  std::vector<sf::Packet> read() override;

  void send(sf::Packet packet) override;

private:
  sf::UdpSocket mUdpSocket;
  sf::IpAddress mUdpIpAddress;
  unsigned short mUdpPort;

};