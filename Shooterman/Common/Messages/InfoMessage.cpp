#include "InfoMessage.h"
#include "../MessageId.h"

InfoMessage::InfoMessage(std::string message, int seconds, int id) {
  mMessage = message;
  mMsgDuration = seconds;
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
  packet << mMsgDuration;
  return packet;
}

void InfoMessage::unpack(sf::Packet packet) {
  packet >> mMessage;
  packet >> mId;
  packet >> mMsgDuration;
}

std::string InfoMessage::getMessage() {
  return mMessage;
}

int InfoMessage::getMsgDuration() {
  return mMsgDuration;
}
