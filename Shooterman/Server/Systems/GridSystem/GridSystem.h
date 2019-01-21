#pragma once

#include <memory>
#include <vector>
#include <SFML\Graphics.hpp>

class GridSystem
{
public:
  GridSystem();
  ~GridSystem();
  std::vector<int> getNearEntities(sf::Vector2i spritePosition);
  std::vector<sf::Vector2i> getNearGridPositions(sf::Vector2i spritePosition);
  void addEntity(int entityId, sf::Vector2i spritePosition);
  void removeEntity(int entityId, sf::Vector2i spritePosition);
  void moveEntity(int entityId, sf::Vector2i oldSpritePosition, sf::Vector2i newSpritePosition);
  bool isOutOfBounds(sf::Vector2i spritePosition);
private:
  std::vector<std::vector<std::vector<int>>> mGrid;
  sf::Vector2i positionToGridPosition(sf::Vector2i position);
  int mGridSize = 20;
  int mGameSize = 1024;
  int mPixelsPerGridBox = mGameSize / mGridSize;
};
