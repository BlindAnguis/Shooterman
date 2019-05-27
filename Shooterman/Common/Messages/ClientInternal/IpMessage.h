#pragma once

#include <SFML/Network.hpp>

#include "../Message.h"
#include "../../MessageId.h"

class IpMessage : Message {
public:
  IpMessage(std::string ip, unsigned short port);
  IpMessage(sf::Packet packet);
  ~IpMessage();

  sf::Packet pack();
  void unpack(sf::Packet packet);

  std::string getIp();
  unsigned short getPort();

private:
  std::string mName = "TRACE";
  std::string mIp;
  unsigned short mPort;
};