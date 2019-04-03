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

  void setPosition(sf::Vector2f position);
  sf::Vector2f getPosition();

  bool hasHealth();
  void setCurrentHealth(int currentHealth);
  int getCurrentHealth();
  void setMaxHealth(int maxHealth);
  int getMaxHealth();

  bool hasMana();
  void setCurrentMana(int currentMana);
  int getCurrentMana();
  void setMaxMana(int maxMana);
  int getMaxMana();

  bool hasStamina();
  void setCurrentStamina(int currentStamina);
  int getCurrentStamina();
  void setMaxStamina(int maxStamina);
  int getMaxStamina();

private:
  int mPlayerId;
  sf::Vector2f mPosition;
  bool mHasHealth = false;
  int mCurrentHealth;
  int mMaxHealth;
  bool mHasMana = false;
  int mCurrentMana;
  int mMaxMana;
  bool mHasStamina = false;
  int mCurrentStamina;
  int mMaxStamina;
};