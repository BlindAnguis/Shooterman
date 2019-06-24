#pragma once

#include "Message.h"

class MapMessage : Message {
public:
  MapMessage(std::string data);
  MapMessage(sf::Packet packet);

  void unpack(sf::Packet packet) override;
  sf::Packet pack() override;

  std::string getData();

private:
  std::string mMapData;
};