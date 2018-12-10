#include "Udp.h"

UdpSocket::UdpSocket(std::string ip, unsigned short port) {
  mName = "UDP";
  mUdpSocket = new sf::UdpSocket();
  mIp = sf::IpAddress(ip);
  mPort = port;
}

UdpSocket::~UdpSocket() {
  delete mUdpSocket;
}

bool UdpSocket::connect() {
  sf::Socket::Status status = mUdpSocket->bind(mPort);

  TRACE_DEBUG("Socket status is: " << status);
  if (status != sf::Socket::Done) {
    TRACE_ERROR("Could not connect to server with address: " << mIp);
    return false;
  }
  else {
    TRACE_INFO("Socket connected");
  }
  return true;
}

void UdpSocket::disconnect() {
  mUdpSocket->unbind();
}

std::vector<sf::Packet> UdpSocket::read() {
  std::vector<sf::Packet> packets;
  sf::Packet packet;
  while (mUdpSocket->receive(packet, mIp, mPort) == sf::Socket::Done) {
    packets.push_back(packet);
  }
  return packets;
}

void UdpSocket::send(sf::Packet packet) {
  mUdpSocket->send(packet, mIp, mPort);
}