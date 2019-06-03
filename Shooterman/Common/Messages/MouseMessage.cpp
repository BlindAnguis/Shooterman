#include "MouseMessage.h"

MouseMessage::MouseMessage() : mPosition(sf::Vector2i()) {}

MouseMessage::MouseMessage(sf::Packet packet) {
  unpack(packet);
}

MouseMessage::MouseMessage(sf::Vector2i position) : mPosition(position) {}
MouseMessage::~MouseMessage() {}

sf::Packet MouseMessage::pack() {
  sf::Packet packet;
  packet << mPosition.x;
  packet << mPosition.y;
  return packet;
}

void MouseMessage::unpack(sf::Packet packet) {
  packet >> mPosition.x;
  packet >> mPosition.y;
}

sf::Vector2i MouseMessage::getPosition() {
  return mPosition;
}