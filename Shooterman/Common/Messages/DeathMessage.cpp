#include "DeathMessage.h"

#include "../MessageId.h"

DeathMessage::DeathMessage(int deadEntity, int killerEntity) {
  mDeadEntity = deadEntity;
  mKillerEntity = killerEntity;
}

DeathMessage::DeathMessage(sf::Packet packet) {
  unpack(packet);
}

DeathMessage::~DeathMessage() {}

void DeathMessage::unpack(sf::Packet packet) {
  packet >> mDeadEntity;
  packet >> mKillerEntity;
}

sf::Packet DeathMessage::pack() {
  sf::Packet packet;
  packet << MessageId::PLAYER_DEATH;
  packet << mDeadEntity;
  packet << mKillerEntity;
  return packet;
}

int DeathMessage::getDeadEntity() {
  return mDeadEntity;
}

int DeathMessage::getKillerEntity() {
  return mKillerEntity;
}
