#pragma once

#include "Message.h"

class MapMessage : Message {
public:
  MapMessage(std::string name, std::string data);
  MapMessage(sf::Packet packet);

  void unpack(sf::Packet packet) override;
  sf::Packet pack() override;

  std::string getName();
  std::string getData();

private:
  std::string mMapName;
  std::string mMapData;
};