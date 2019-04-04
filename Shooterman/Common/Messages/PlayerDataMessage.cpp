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

  packet << mPosition.x;
  packet << mPosition.y;
  packet << mGlobalBounds.width;
  packet << mGlobalBounds.height;

  packet << mHasHealth;
  packet << mCurrentHealth;
  packet << mMaxHealth;

  packet << mHasMana;
  packet << mCurrentMana;
  packet << mMaxMana;

  packet << mHasStamina;
  packet << mCurrentStamina;
  packet << mMaxStamina;
  return packet;
}

void PlayerDataMessage::unpack(sf::Packet packet) {
  packet >> mPlayerId;

  packet >> mPosition.x;
  packet >> mPosition.y;
  packet >> mGlobalBounds.width;
  packet >> mGlobalBounds.height;

  packet >> mHasHealth;
  packet >> mCurrentHealth;
  packet >> mMaxHealth;

  packet >> mHasMana;
  packet >> mCurrentMana;
  packet >> mMaxMana;

  packet >> mHasStamina;
  packet >> mCurrentStamina;
  packet >> mMaxStamina;
}

bool PlayerDataMessage::hasHealth() {
  return mHasHealth;
}

void PlayerDataMessage::setPosition(sf::Vector2f position) {
  mPosition = position;
}

sf::Vector2f PlayerDataMessage::getPosition() {
  return mPosition;
}

void PlayerDataMessage::setGlobalBounds(GlobalBounds globalBounds) {
  mGlobalBounds = globalBounds;
}

GlobalBounds PlayerDataMessage::getGlobalBounds() {
  return mGlobalBounds;
}


void PlayerDataMessage::setCurrentHealth(int currentHealth) {
  mCurrentHealth = currentHealth;
}

int PlayerDataMessage::getCurrentHealth() {
  return mCurrentHealth;
}

void PlayerDataMessage::setMaxHealth(int maxHealth) {
  mMaxHealth = maxHealth;
}

int PlayerDataMessage::getMaxHealth() {
  return mMaxHealth;
}


bool PlayerDataMessage::hasMana() {
  return mHasMana;
}

void PlayerDataMessage::setCurrentMana(int currentMana) {
  mHasMana = true;
  mCurrentMana = currentMana;
}

int PlayerDataMessage::getCurrentMana() {
  return mCurrentMana;
}

void PlayerDataMessage::setMaxMana(int maxMana) {
  mMaxMana = maxMana;
}

int PlayerDataMessage::getMaxMana() {
  return mMaxMana;
}


bool PlayerDataMessage::hasStamina() {
  return mHasStamina;
}

void PlayerDataMessage::setCurrentStamina(int currentStamina) {
  mHasStamina = true;
  mCurrentStamina = currentStamina;
}

int PlayerDataMessage::getCurrentStamina() {
  return mCurrentStamina;
}

void PlayerDataMessage::setMaxStamina(int maxStamina) {
  mMaxStamina = maxStamina;
}

int PlayerDataMessage::getMaxStamina() {
  return mMaxStamina;
}