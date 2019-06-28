#include "ChangeUsernameMessage.h"

#include "../MessageId.h"

ChangeUsernameMessage::ChangeUsernameMessage(std::string username, int id) {
  mUsername = username;
  mId = id;
}

ChangeUsernameMessage::ChangeUsernameMessage(sf::Packet packet) {
  unpack(packet);
}

void ChangeUsernameMessage::unpack(sf::Packet packet) {
  packet >> mUsername;
  packet >> mId;
}

sf::Packet ChangeUsernameMessage::pack() {
  sf::Packet packet;
  packet << MessageId::NEW_USERNAME;
  packet << mUsername;
  packet << mId;
  return packet;
}

std::string ChangeUsernameMessage::getUsername() {
  return mUsername;
}

void ChangeUsernameMessage::setId(int id) {
  mId = id;
}

int ChangeUsernameMessage::getId() {
  return mId;
}