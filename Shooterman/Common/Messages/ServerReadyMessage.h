#pragma once

#include "Message.h"


class ServerReadyMessage : Message {
public:
  ServerReadyMessage();
  ~ServerReadyMessage();
  sf::Packet pack() override;
  void unpack(sf::Packet packet) override;
};


