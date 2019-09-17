#pragma once

#include "Message.h"

class DeathMessage : public Message {
public:
  DeathMessage(int deadEntity, int killerEntity);
  DeathMessage(sf::Packet packet);
  ~DeathMessage();

  void unpack(sf::Packet packet) override;
  sf::Packet pack() override;

  int getDeadEntity();
  int getKillerEntity();

private:
  int mDeadEntity;
  int mKillerEntity;
};

