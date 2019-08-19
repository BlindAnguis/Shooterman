#include "Map.h"

#include <string>
#include <sstream>

#include <SFML/Graphics/Image.hpp>

#include "GUI/Resources/GuiResourceManager.h"

Map::Map() : mNumberOfPlayers(0) {}

Map::~Map() {}

void Map::setTile(int x, int y, Textures tileId, sf::Sprite sprite) {
  if ((x == 0 || y == 0 || x == 31 || y == 31) &&
    !(tileId == Textures::HorizontalWall1 || tileId == Textures::VerticalWall1 || tileId == Textures::FloorSpikes)) {
    // Only these are allowed on the edges
    return;
  }

  if (mMapMatrix[x][y].first == Textures::SpawnPoint) {
    --mNumberOfPlayers;
  }

  if (tileId == Textures::SpawnPoint) {
    ++mNumberOfPlayers;
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

  mNumberOfPlayers = 0;
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
      if (mMapMatrix[x][y].first == Textures::SpawnPoint) {
        ++mNumberOfPlayers;
      }
      ++x;
    }
    x = 0;
    ++y;
  }
}

sf::Texture Map::generateThumbnail() {
  sf::Image image;
  image.create(32, 32, sf::Color::Magenta);

  for (int y = 0; y < 32; ++y) {
    for (int x = 0; x < 32; ++x) {
      switch (mMapMatrix[x][y].first) {
      case Textures::FloorWhole:
      case Textures::FloorCracked:
        image.setPixel(x, y, sf::Color(0, 120, 0));
        break;
      case Textures::HorizontalWall1:
      case Textures::VerticalWall1:
        image.setPixel(x, y, sf::Color(0, 0, 0));
        break;
      case Textures::FloorSpikes:
        image.setPixel(x, y, sf::Color(200, 0, 0));
        break;
      case Textures::SpawnPoint:
        image.setPixel(x, y, sf::Color(0, 0, 200));
      }
    }
  }

  sf::Texture texture;
  texture.loadFromImage(image);
  return texture;
}
