#pragma once

#include <SFML/Network.hpp>

#include "../Message.h"
#include "../../MessageId.h"

class IpMessage : Message {
public:
  IpMessage(std::string ip, unsigned short port) : mIp(ip), mPort(port) { }

  IpMessage(sf::Packet packet) {
    packet >> mIp;
    packet >> mPort;
  }

  ~IpMessage() {}

  sf::Packet pack() {
    sf::Packet packet;
    packet << IP_MESSAGE;
    packet << mIp;
    packet << mPort;
    return packet;
  }

  void unpack(sf::Packet packet) {
    packet >> mIp;
    packet >> mPort;
  }

  std::string getIp() {
    return mIp;
  }

  unsigned short getPort() {
    return mPort;
  }

private:
  std::string mName = "TRACE";
  std::string mIp;
  unsigned short mPort;
};