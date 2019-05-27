#pragma once

#include "Message.h"
#include "../MessageId.h"

class SetupSocketConnectionData : public Message {
public:
  SetupSocketConnectionData();
  SetupSocketConnectionData(std::string ip, unsigned short port);

  sf::Packet pack();

  void unpack(sf::Packet packet);

  std::string getIP();
  unsigned short getPort();

private:
  std::string mIP;
  unsigned short mPort;
};