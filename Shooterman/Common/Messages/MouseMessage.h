#pragma once
#pragma once
#include "Message.h"
#include "../MessageId.h"

class MouseMessage : Message {
public:
  MouseMessage() : mPosition(sf::Vector2i()) {}
  MouseMessage(sf::Vector2i position) : mPosition(position) {}
  ~MouseMessage() {}

  sf::Packet pack() {
    sf::Packet packet;
    packet << mPosition.x;
    packet << mPosition.y;
    return packet;
  }

  void unpack(sf::Packet packet) {
    packet >> mPosition.x;
    packet >> mPosition.y;
  }

  sf::Vector2i getPosition() {
    return mPosition;
  }

private:
  std::string mName = "TRACE";
  sf::Vector2i mPosition;
};