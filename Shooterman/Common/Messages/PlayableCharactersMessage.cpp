#include "PlayableCharactersMessage.h"

#include "../MessageId.h"

PlayableCharactersMessage::PlayableCharactersMessage() {}

PlayableCharactersMessage::PlayableCharactersMessage(sf::Packet packet) {
  unpack(packet);
}

void PlayableCharactersMessage::unpack(sf::Packet packet) {
  int nrOfPlayerClasses = 0;
  packet >> nrOfPlayerClasses;
  for (int i = 0; i < nrOfPlayerClasses; ++i) {
    sf::Int16 playerClassNr;
    packet >> playerClassNr;
    mPlayerClasses.push_back((PlayerClass)playerClassNr);
  }
}

sf::Packet PlayableCharactersMessage::pack() {
  sf::Packet packet;
  packet << PLAYABLE_CHARACTERS;
  packet << mPlayerClasses.size();
  for (auto playerClass : mPlayerClasses) {
    packet << (sf::Int16)playerClass;
  }
  return packet;
}

void PlayableCharactersMessage::addPlayerClass(PlayerClass playerClass) {
  mPlayerClasses.push_back(playerClass);
}

std::vector<PlayerClass> PlayableCharactersMessage::getPlayerClasses() {
  return mPlayerClasses;
}
