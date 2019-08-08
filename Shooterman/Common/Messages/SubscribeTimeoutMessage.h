#pragma once
#include "Message.h"
#include "../MessageId.h"

class SuscribeTimeoutMessage : Message {
public:
  sf::Packet pack();
  void unpack(sf::Packet packet);
};