#include "ClientMainAndNetworkHandlerMessages.h"

SetupSocketConnectionData::SetupSocketConnectionData() {}
SetupSocketConnectionData::SetupSocketConnectionData(std::string ip, unsigned short port) : mIP(ip), mPort(port) {}

sf::Packet SetupSocketConnectionData::pack() {
  sf::Packet packet;
  //packet << SETUP_SOCKET_CONNECTION_DATA;
  packet << mIP;
  packet << mPort;
  return packet;
}

void SetupSocketConnectionData::unpack(sf::Packet packet) {
  packet >> mIP;
  packet >> mPort;
}

std::string SetupSocketConnectionData::getIP() { 
  return mIP; 
}

unsigned short SetupSocketConnectionData::getPort() { 
  return mPort; 
}