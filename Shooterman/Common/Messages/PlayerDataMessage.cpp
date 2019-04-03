#include "PlayerDataMessage.h"

PlayerDataMessage::PlayerDataMessage() {}
PlayerDataMessage::PlayerDataMessage(int playerId) : mPlayerId(playerId) {}

PlayerDataMessage::~PlayerDataMessage() {}

int PlayerDataMessage::getPlayerId() {
  return mPlayerId;
}

sf::Packet PlayerDataMessage::pack() {
  sf::Packet packet;
  packet << PLAYER_DATA;
  packet << mPlayerId;
  packet << mCurrentHealth;
  packet << mCurrentMana;
  return packet;
}

void PlayerDataMessage::unpack(sf::Packet packet) {
  packet >> mPlayerId;
  packet >> mCurrentHealth;
  packet >> mCurrentMana;
}

void PlayerDataMessage::setCurrentHealth(int currentHealth) {
  mCurrentHealth = currentHealth;
}

int PlayerDataMessage::getCurrentHealth() {
  return mCurrentHealth;
}

void PlayerDataMessage::setCurrentMana(int currentMana) {
  mCurrentMana = currentMana;
}

int PlayerDataMessage::getCurrentMana() {
  return mCurrentMana;
}