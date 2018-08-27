#pragma once

#include <SFML/Network.hpp>

class Message {
  virtual sf::Packet pack() = 0;
  virtual void unpack(sf::Packet packet) = 0;
};