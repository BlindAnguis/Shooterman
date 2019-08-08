#pragma once
#include "Message.h"
#include "../MessageId.h"

class SubscribeDoneMessage : Message {
public:
  sf::Packet pack();
  void unpack(sf::Packet packet);
};