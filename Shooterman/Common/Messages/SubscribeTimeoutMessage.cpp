#include "SubscribeTimeoutMessage.h"

sf::Packet SuscribeTimeoutMessage::pack() {
  sf::Packet packet;
  packet << MessageId::SUBSCRIBE_TIMEOUT;
  return packet;
}

void SuscribeTimeoutMessage::unpack(sf::Packet packet) {}
