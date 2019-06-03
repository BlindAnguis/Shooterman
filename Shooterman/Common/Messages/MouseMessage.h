#pragma once
#include "Message.h"
#include "../MessageId.h"

class MouseMessage : Message {
public:
  MouseMessage();
  MouseMessage(sf::Packet packet);
  MouseMessage(sf::Vector2i position);
  ~MouseMessage();

  sf::Packet pack();
  void unpack(sf::Packet packet);

  sf::Vector2i getPosition();

private:
  sf::Vector2i mPosition;
};