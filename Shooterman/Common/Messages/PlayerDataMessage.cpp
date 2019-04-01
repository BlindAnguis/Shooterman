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
  return packet;
}

void PlayerDataMessage::unpack(sf::Packet packet) {
  packet >> mPlayerId;
  packet >> mCurrentHealth;
}

void PlayerDataMessage::setCurrentHealth(int currentHealth) {
  mCurrentHealth = currentHealth;
}

int PlayerDataMessage::getCurrentHealth() {
  return mCurrentHealth;
}