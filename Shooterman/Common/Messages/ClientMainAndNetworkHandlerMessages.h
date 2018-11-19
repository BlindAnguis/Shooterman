#pragma once

#include "Message.h"
#include "../MessageId.h"

class SetupSocketConnectionData : public Message {
public:
  SetupSocketConnectionData() {}
  SetupSocketConnectionData(std::string ip, unsigned short port) : mIP(ip), mPort(port) {}

  sf::Packet pack() {
    sf::Packet packet;
    packet << SETUP_SOCKET_CONNECTION_DATA;
    packet << mIP;
    packet << mPort;
    return packet;
  }

  void unpack(sf::Packet packet) {
    packet >> mIP;
    packet >> mPort;
  }

  std::string getIP() { return mIP; }
  unsigned short getPort() { return mPort; }

private:
  std::string mIP;
  unsigned short mPort;
};