#include "LobbyDataMessage.h"

LobbyDataMessage::LobbyDataMessage() { }
LobbyDataMessage::LobbyDataMessage(sf::Packet packet) {
  unpack(packet);
}

sf::Packet LobbyDataMessage::pack() {
  sf::Packet packet;
  packet << LOBBY_DATA;
  packet << mPlayerNames.size();
  for (auto playerName : mPlayerNames) {
    packet << playerName;
  }
  return packet;
}

void LobbyDataMessage::unpack(sf::Packet packet) {
  int numberOfPlayerNames = 0;
  packet >> numberOfPlayerNames;

  for (int i = 0; i < numberOfPlayerNames; i++) {
    std::string playerName;
    packet >> playerName;
    mPlayerNames.push_back(playerName);
  }
}

void LobbyDataMessage::addPlayerName(std::string playerName) {
  mPlayerNames.push_back(playerName);
}

std::list<std::string> LobbyDataMessage::getPlayerNames() {
  return mPlayerNames;
}