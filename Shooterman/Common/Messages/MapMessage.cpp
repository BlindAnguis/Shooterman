#include "MapMessage.h"

#include "../MessageId.h"

MapMessage::MapMessage(std::string name, std::string data) : mMapName(name), mMapData(data) { }

MapMessage::MapMessage(sf::Packet packet) {
  unpack(packet);
}

void MapMessage::unpack(sf::Packet packet) {
  packet >> mMapName;
  packet >> mMapData;
}

sf::Packet MapMessage::pack() {
  sf::Packet packet;
  packet << MessageId::MAP_DATA;
  packet << mMapName;
  packet << mMapData;
  return packet;
}

std::string MapMessage::getName() {
  return mMapName;
}

std::string MapMessage::getData() {
  return mMapData;
}
