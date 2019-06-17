#pragma once

#include "Message.h"
#include "../../Server/Components/Components.h"

class CharacherChoosenMessage : public Message {
public:
  CharacherChoosenMessage(PlayerClass playerClass);
  CharacherChoosenMessage(sf::Packet packet);

  void unpack(sf::Packet packet) override;
  sf::Packet pack() override;

  void setId(int id);
  int getId();

  PlayerClass getPlayerClass();

private:
  PlayerClass mPlayerClass;
  int mId;
};