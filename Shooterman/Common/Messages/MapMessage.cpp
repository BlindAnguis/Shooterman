#include "MapMessage.h"

#include "../MessageId.h"

MapMessage::MapMessage(std::string data) : mMapData(data) { }

MapMessage::MapMessage(sf::Packet packet) {
  unpack(packet);
}

void MapMessage::unpack(sf::Packet packet) {
  packet >> mMapData;
}

sf::Packet MapMessage::pack() {
  sf::Packet packet;
  packet << MessageId::MAP_DATA;
  packet << mMapData;
  return packet;
}

std::string MapMessage::getData() {
  return mMapData;
}
