#include "PlayerDataMessage.h"

PlayerDataMessage::PlayerDataMessage() { }

PlayerDataMessage::PlayerDataMessage(sf::Packet packet) {
  unpack(packet);
}

PlayerDataMessage::~PlayerDataMessage() { }

sf::Packet PlayerDataMessage::pack() {
  sf::Packet packet;
  packet << PLAYER_DATA;
  packet << mPlayerDatas.size();

  for (PlayerData playerData : mPlayerDatas) {
    packet << playerData.hasHealth;
    packet << playerData.currentHealth;
    packet << playerData.maxHealth;
    packet << playerData.hasStamina;
    packet << playerData.currentStamina;
    packet << playerData.maxStamina;
    packet << playerData.hasMana;
    packet << playerData.currentMana;
    packet << playerData.maxMana;
    packet << playerData.position.x;
    packet << playerData.position.y;
    packet << playerData.globalBounds.width;
    packet << playerData.globalBounds.height;
  }

  return packet;
}

void PlayerDataMessage::unpack(sf::Packet packet) {
  packet >> mNumberOfPlayerData;

  for (int i = 0; i < mNumberOfPlayerData; ++i) {
    PlayerData playerData;
    packet >> playerData.hasHealth;
    packet >> playerData.currentHealth;
    packet >> playerData.maxHealth;
    packet >> playerData.hasStamina;
    packet >> playerData.currentStamina;
    packet >> playerData.maxStamina;
    packet >> playerData.hasMana;
    packet >> playerData.currentMana;
    packet >> playerData.maxMana;
    packet >> playerData.position.x;
    packet >> playerData.position.y;
    packet >> playerData.globalBounds.width;
    packet >> playerData.globalBounds.height;
    mPlayerDatas.push_back(playerData);
  }
}

void PlayerDataMessage::addPlayerData(PlayerData playerData) {
  mPlayerDatas.push_back(playerData);
}

PlayerData PlayerDataMessage::getPlayerData(int position) {
  return mPlayerDatas[position];
}

int PlayerDataMessage::getNumberOfPlayerData() {
  return mPlayerDatas.size();
}