#include "IpMessage.h"

IpMessage::IpMessage(std::string ip, unsigned short port) : mIp(ip), mPort(port) {
}

IpMessage::IpMessage(sf::Packet packet) {
  packet >> mIp;
  packet >> mPort;
}

IpMessage::~IpMessage() {}

sf::Packet IpMessage::pack() {
  sf::Packet packet;
  packet << MessageId::IP_MESSAGE;
  packet << mIp;
  packet << mPort;
  return packet;
}

void IpMessage::unpack(sf::Packet packet) {
  packet >> mIp;
  packet >> mPort;
}

std::string IpMessage::getIp() {
  return mIp;
}

unsigned short IpMessage::getPort() {
  return mPort;
}