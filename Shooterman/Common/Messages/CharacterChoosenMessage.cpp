#include "CharacterChoosenMessage.h"
#include "../MessageId.h"

CharacherChoosenMessage::CharacherChoosenMessage(PlayerClass playerClass) : mPlayerClass(playerClass) {}

CharacherChoosenMessage::CharacherChoosenMessage(sf::Packet packet) {
  unpack(packet);
}

void CharacherChoosenMessage::unpack(sf::Packet packet) {
  sf::Int16 playerClassNr;
  packet >> playerClassNr;
  packet >> mId;
  mPlayerClass = (PlayerClass)playerClassNr;
}

sf::Packet CharacherChoosenMessage::pack() {
  sf::Packet packet;
  packet << MessageId::CHARACTER_CHOOSEN;
  packet << (sf::Int16)mPlayerClass;
  packet << mId;
  return packet;
}

void CharacherChoosenMessage::setId(int id) {
  mId = id;
}

int CharacherChoosenMessage::getId() {
  return mId;
}

PlayerClass CharacherChoosenMessage::getPlayerClass() {
  return mPlayerClass;
}
