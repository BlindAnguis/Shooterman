#pragma once

#include "Message.h"
#include "../MessageId.h"

struct GlobalBounds {
  float width;
  float height;
};

struct PlayerData {
  bool hasHealth = false;
  int currentHealth;
  int maxHealth;
  bool hasStamina = false;
  int currentStamina;
  int maxStamina;
  bool hasMana = false;
  int currentMana;
  int maxMana;
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

  unsigned int getNumberOfPlayerData();

private:
  int mNumberOfPlayerData = 0;
  std::vector<PlayerData> mPlayerDatas;
};