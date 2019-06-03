#pragma once

#include "Message.h"

class ChangeUsernameMessage : Message {
public:
  ChangeUsernameMessage(std::string username, int id = -1);
  ChangeUsernameMessage(sf::Packet packet);

  void unpack(sf::Packet packet) override;
  sf::Packet pack() override;

  std::string getUsername();

  void setId(int id);
  int getId();

private:
  int mId;
  std::string mUsername;
};