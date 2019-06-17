#pragma once

#include "Message.h"

#include "../../Server/Components/Components.h"

class PlayableCharactersMessage : Message {
public:
  PlayableCharactersMessage();
  PlayableCharactersMessage(sf::Packet packet);

  void unpack(sf::Packet packet) override;
  sf::Packet pack() override;

  void addPlayerClass(PlayerClass playerClass);
  std::vector<PlayerClass> getPlayerClasses();

private:
  std::vector<PlayerClass> mPlayerClasses;
};