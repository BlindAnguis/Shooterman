#pragma once

#include <list>

#include "Message.h"
#include "../MessageId.h"

class LobbyDataMessage : Message {
public:
  LobbyDataMessage();

  sf::Packet pack();

  void unpack(sf::Packet packet);

  void addPlayerName(std::string playerName);
  std::list<std::string> getPlayerNames();

private:
  std::list<std::string> mPlayerNames;
};