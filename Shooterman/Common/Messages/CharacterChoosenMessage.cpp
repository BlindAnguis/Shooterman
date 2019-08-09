#include "CharacterChoosenMessage.h"
#include "../MessageId.h"

CharacterChoosenMessage::CharacterChoosenMessage(PlayerClass playerClass) : mPlayerClass(playerClass) {}

CharacterChoosenMessage::CharacterChoosenMessage(sf::Packet packet) {
  unpack(packet);
}

void CharacterChoosenMessage::unpack(sf::Packet packet) {
  sf::Int16 playerClassNr;
  packet >> playerClassNr;
  packet >> mId;
  mPlayerClass = (PlayerClass)playerClassNr;
}

sf::Packet CharacterChoosenMessage::pack() {
  sf::Packet packet;
  packet << MessageId::CHARACTER_CHOOSEN;
  packet << (sf::Int16)mPlayerClass;
  packet << mId;
  return packet;
}

void CharacterChoosenMessage::setId(int id) {
  mId = id;
}

int CharacterChoosenMessage::getId() {
  return mId;
}

PlayerClass CharacterChoosenMessage::getPlayerClass() {
  return mPlayerClass;
}
