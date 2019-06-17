#pragma once

#include <SFML/Network.hpp>

#include "../Message.h"
#include "../../MessageId.h"

class ClientDisconnectedMessage : Message {
public:
  ClientDisconnectedMessage(int id);
  ClientDisconnectedMessage(sf::Packet packet);
  ~ClientDisconnectedMessage();

  sf::Packet pack();
  void unpack(sf::Packet packet);

  int getId();

private:
  std::string mName = "TRACE";
  int mId;
};