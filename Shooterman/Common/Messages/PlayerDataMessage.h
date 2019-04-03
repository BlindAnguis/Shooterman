#pragma once

#include "Message.h"
#include "../MessageId.h"

class PlayerDataMessage : Message {
public:
  PlayerDataMessage();
  PlayerDataMessage(int playerId);
  ~PlayerDataMessage();

  int getPlayerId();

  sf::Packet pack() override;
  void unpack(sf::Packet packet) override;

  void setCurrentHealth(int currentHealth);
  int getCurrentHealth();

  void setCurrentMana(int currentMana);
  int getCurrentMana();

private:
  int mPlayerId;
  int mCurrentHealth;
  int mCurrentMana;
};