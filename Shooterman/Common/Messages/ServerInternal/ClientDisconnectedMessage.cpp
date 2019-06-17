#include "ClientDisconnectedMessage.h"

ClientDisconnectedMessage::ClientDisconnectedMessage(int id) : mId(id) { }

ClientDisconnectedMessage::ClientDisconnectedMessage(sf::Packet packet) {
  unpack(packet);
}

ClientDisconnectedMessage::~ClientDisconnectedMessage() {}

sf::Packet ClientDisconnectedMessage::pack() {
  sf::Packet packet;
  packet << CLIENT_DISCONNECTED;
  packet << mId;
  return packet;
}

void ClientDisconnectedMessage::unpack(sf::Packet packet) {
  packet >> mId;
}

int ClientDisconnectedMessage::getId() {
  return mId;
}