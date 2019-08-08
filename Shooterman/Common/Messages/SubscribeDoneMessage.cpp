#include "SubscribeDoneMessage.h"

sf::Packet SubscribeDoneMessage::pack() {
  sf::Packet packet;
  packet << MessageId::SUBSCRIBE_DONE;
  return packet;
}

void SubscribeDoneMessage::unpack(sf::Packet packet) {}
