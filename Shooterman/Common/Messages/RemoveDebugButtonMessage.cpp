#include "RemoveDebugButtonMessage.h"

#include "../MessageId.h"

RemoveDebugButtonMessage::RemoveDebugButtonMessage(int subscriberId) 
  : mSubscruberId(subscriberId) {
}

RemoveDebugButtonMessage::RemoveDebugButtonMessage(sf::Packet packet) {
  unpack(packet);
}

sf::Packet RemoveDebugButtonMessage::pack() {
  sf::Packet packet;
  packet << REMOVE_DEBUG_BUTTON;
  packet << mSubscruberId;
  return packet;
}

void RemoveDebugButtonMessage::unpack(sf::Packet packet) {
  packet >> mSubscruberId;
}

int RemoveDebugButtonMessage::getSubscriberId() {
  return mSubscruberId;
}
