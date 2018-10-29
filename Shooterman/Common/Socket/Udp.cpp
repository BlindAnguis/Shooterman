#include "Udp.h"

UdpSocket::UdpSocket() {
  mName = "UDP";
}

bool UdpSocket::connect(std::string ip, unsigned short port) {
  mUdpPort = port;
  sf::Socket::Status status = mUdpSocket.bind(port);
  mUdpIpAddress = sf::IpAddress(ip);

  TRACE_DEBUG("Socket status is: " << status);
  if (status != sf::Socket::Done) {
    TRACE_ERROR("Could not connect to server with address: " << ip);
    return false;
  }
  else {
    TRACE_INFO("Socket connected");
  }
  return true;
}

void UdpSocket::disconnect() {

}

std::vector<sf::Packet> UdpSocket::read() {
  std::vector<sf::Packet> packets;
  sf::Packet packet;
  while (mUdpSocket.receive(packet, mUdpIpAddress, mUdpPort) == sf::Socket::Done) {
    packets.push_back(packet);
  }
  return packets;
}

void UdpSocket::send(sf::Packet packet) {
  mUdpSocket.send(packet, mUdpIpAddress, mUdpPort);
}