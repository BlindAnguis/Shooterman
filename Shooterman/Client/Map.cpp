#include "Map.h"

#include <string>
#include <sstream>

#include "GUI/Resources/GuiResourceManager.h"

Map::Map() {}

Map::~Map() {}

void Map::setTile(int x, int y, Textures tileId, sf::Sprite sprite) {
  if ((x == 0 || y == 0 || x == 31 || y == 31) && 
    !(tileId == Textures::HorizontalWall1 || tileId == Textures::VerticalWall1 || tileId == Textures::FloorSpikes)) {
    // Only these are allowed on the edges
    return;
  }

  mMapMatrix[x][y] = std::make_pair(tileId, sprite);
}

std::pair<Textures, sf::Sprite> Map::getTile(int x, int y) {
  return mMapMatrix[x][y];
}

void Map::clear() {
  mMapMatrix = std::array<std::array<std::pair<Textures, sf::Sprite>, 32>, 32>();
}

void Map::createDefaultMap() {
  sf::Sprite wall = GuiResourceManager::getInstance().createSprite(Textures::HorizontalWall1);
  sf::Sprite floor = GuiResourceManager::getInstance().createSprite(Textures::FloorWhole);

  for (int x = 0; x < 32; ++x) {
    for (int y = 0; y < 32; ++y) {
      if (x == 0 || y == 0 || x == 31 || y == 31) {
        mMapMatrix[x][y] = std::make_pair(Textures::HorizontalWall1, wall);
      } else {
        mMapMatrix[x][y] = std::make_pair(Textures::FloorWhole, floor);
      }
    }
  }
}

std::string Map::toString() {
  std::string data;
  for (int y = 0; y < 32; ++y) {
    for (int x = 0; x < 32; ++x) {
      data += std::to_string((int)mMapMatrix[x][y].first);
      data += " ";
    }
    data += "\n";
  }

  return data;
}

void Map::fromString(std::string data) {
  mMapMatrix = std::array<std::array<std::pair<Textures, sf::Sprite>, 32>, 32>();

  int x = 0;
  int y = 0;

  std::stringstream lineSs(data);
  std::string line;

  while (std::getline(lineSs, line, '\n')) {
    std::stringstream idSs(line);
    std::string idAsString;
    while (std::getline(idSs, idAsString, ' ')) {
      mMapMatrix[x][y] = std::make_pair((Textures)std::stoi(idAsString), GuiResourceManager::getInstance().createSprite((Textures)std::stoi(idAsString)));
      ++x;
    }
    x = 0;
    ++y;
  }
}
