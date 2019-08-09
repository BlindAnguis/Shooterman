#pragma once

#include "Message.h"
#include "../../Server/Components/Components.h"

class CharacterChoosenMessage : public Message {
public:
  CharacterChoosenMessage(PlayerClass playerClass);
  CharacterChoosenMessage(sf::Packet packet);

  void unpack(sf::Packet packet) override;
  sf::Packet pack() override;

  void setId(int id);
  int getId();

  PlayerClass getPlayerClass();

private:
  PlayerClass mPlayerClass;
  int mId;
};