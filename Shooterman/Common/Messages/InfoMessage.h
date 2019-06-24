#pragma once

#include "Message.h"

class InfoMessage : Message {
public:
  InfoMessage(std::string message, int id = -1);
  InfoMessage(sf::Packet packet);

  void unpack(sf::Packet packet) override;
  sf::Packet pack() override;

  std::string getMessage();

private:
  int mId;
  std::string mMessage;
};