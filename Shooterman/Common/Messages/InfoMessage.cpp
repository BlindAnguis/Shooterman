#include "InfoMessage.h"
#include "../MessageId.h"

InfoMessage::InfoMessage(std::string message, int id) {
  mMessage = message;
  mId = id;
}

InfoMessage::InfoMessage(sf::Packet packet) {
  unpack(packet);
}

sf::Packet InfoMessage::pack() {
  sf::Packet packet;
  packet << INFO_MESSAGE;
  packet << mMessage;
  packet << mId;
  
  return packet;
}

void InfoMessage::unpack(sf::Packet packet) {
  packet >> mMessage;
  packet >> mId;
}

std::string InfoMessage::getMessage() {
  return mMessage;
}
