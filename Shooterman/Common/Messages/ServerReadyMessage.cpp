#include "ServerReadyMessage.h"

#include "../MessageId.h"

ServerReadyMessage::ServerReadyMessage() {}
ServerReadyMessage::~ServerReadyMessage() {}

sf::Packet ServerReadyMessage::pack() {
  sf::Packet packet;
  packet << MessageId::SERVER_READY;
  return packet;
}

void ServerReadyMessage::unpack(sf::Packet packet) {
  
}