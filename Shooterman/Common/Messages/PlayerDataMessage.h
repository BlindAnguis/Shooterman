#pragma once

#include "Message.h"
#include "../MessageId.h"

struct GlobalBounds {
  float width;
  float height;
};

struct PlayerData {
  bool hasHealth = false;
  float currentHealth;
  float maxHealth;
  bool hasStamina = false;
  float currentStamina;
  float maxStamina;
  bool hasMana = false;
  float currentMana;
  float maxMana;
  sf::Vector2f position;
  GlobalBounds globalBounds;
};

class PlayerDataMessage : Message {
public:
  PlayerDataMessage();
  PlayerDataMessage(sf::Packet packet);
  ~PlayerDataMessage();

  sf::Packet pack() override;
  void unpack(sf::Packet packet) override;

  void addPlayerData(PlayerData playerData);
  PlayerData getPlayerData(int position);

  int getNumberOfPlayerData();

private:
  int mNumberOfPlayerData = 0;
  std::vector<PlayerData> mPlayerDatas;
};