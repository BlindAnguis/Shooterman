#include "SocketImpl.h"

#include <SFML/Network/IpAddress.hpp>

SocketImpl::SocketImpl() {}

SocketImpl::~SocketImpl() {}

Soc::Status SocketImpl::connect(std::string ipAddress, std::uint16_t port, int timeout) {
  return (Soc::Status)mSocket.connect(sf::IpAddress(ipAddress), port, sf::milliseconds(timeout));
}

void SocketImpl::disconnect() {
  mSocket.disconnect();
}

void SocketImpl::setBlocking(bool blocking) {
  mSocket.setBlocking(blocking);
}

void SocketImpl::send(sf::Packet& packet) {
  mSocket.send(packet);
}

Soc::Status SocketImpl::receive(sf::Packet& packet) {
  return (Soc::Status)mSocket.receive(packet);
}
