#pragma once

#include "Message.h"

class RemoveDebugButtonMessage : Message {
public:
  RemoveDebugButtonMessage(int subscriberId = -1);
  RemoveDebugButtonMessage(sf::Packet packet);

  sf::Packet pack();
  void unpack(sf::Packet packet);

  int getSubscriberId();

private:
  int mSubscruberId;
};