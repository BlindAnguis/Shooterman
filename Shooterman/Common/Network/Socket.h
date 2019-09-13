#pragma once

#include <string>

#include <SFML/Network/Packet.hpp>

namespace Soc {
  enum Status {
    Done,         ///< The socket has sent / received the data
    NotReady,     ///< The socket is not ready to send / receive data yet
    Partial,      ///< The socket sent a part of the data
    Disconnected, ///< The TCP socket has been disconnected
    Error         ///< An unexpected error happened
  };
}

class Socket {
public:
  virtual ~Socket() { };

  virtual Soc::Status connect(std::string ipAddress, std::uint16_t port, int timeout) = 0;
  virtual void disconnect() = 0;

  virtual void setBlocking(bool blocking) = 0;

  virtual void send(sf::Packet& packet) = 0;
  virtual Soc::Status receive(sf::Packet& packet) = 0;

};