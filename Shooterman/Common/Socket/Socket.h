#pragma once

#include <vector>

#include <SFML/Network.hpp>

#include "../Trace.h"

class Socket : public Trace {
public:

  virtual bool connect() = 0;

  virtual void disconnect() = 0;

  virtual std::vector<sf::Packet> read() = 0;

  void send(std::vector<sf::Packet> packets);

  virtual void send(sf::Packet packet) = 0;

protected:
  sf::IpAddress mIp;
  unsigned short mPort;
};