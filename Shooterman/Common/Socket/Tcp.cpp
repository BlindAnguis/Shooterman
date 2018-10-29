#include "Tcp.h"

TcpSocket::TcpSocket() {}

bool TcpSocket::connect(std::string ip, unsigned short port) {
  sf::Socket::Status status = mTcpSocket.connect(ip, port);
  mTcpSocket.setBlocking(false);

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

void TcpSocket::disconnect() {

}

std::vector<sf::Packet> TcpSocket::read() {
  std::vector<sf::Packet> packets;
  sf::Packet packet;
  while (mTcpSocket.receive(packet) == sf::Socket::Done) {
    packets.push_back(packet);
  }
  return packets;
}

void TcpSocket::send(sf::Packet packet) {
  mTcpSocket.send(packet);
}