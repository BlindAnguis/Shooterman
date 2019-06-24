#pragma once

#include <array>

#include <SFML/Graphics/Sprite.hpp>

#include "../Common/Textures.h"

class Map {
public:
  Map();
  ~Map();

  void setTile(int x, int y, Textures tileId, sf::Sprite sprite);
  std::pair<Textures, sf::Sprite> getTile(int x, int y);

  void clear();
  void createDefaultMap();

  std::string toString();
  void fromString(std::string data);

private:
  std::array<std::array<std::pair<Textures, sf::Sprite>, 32>, 32> mMapMatrix;
};