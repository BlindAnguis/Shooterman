#pragma once

#include <memory>
#include <vector>
#include <SFML\Graphics.hpp>
#include "../../../Common/Trace.h"

class GridSystem : Trace
{
public:
  GridSystem();
  ~GridSystem();
  static GridSystem& get();
  std::vector<int> getNearEntities(sf::Vector2i spritePosition);
  std::vector<sf::Vector2i> getNearGridPositions(sf::Vector2i spritePosition);
  std::vector<sf::Vector2i> getEmptyGridPositions();
  void addEntity(int entityId, sf::Vector2i spritePosition);
  void removeEntity(int entityId, sf::Vector2i spritePosition);
  void moveEntity(int entityId, sf::Vector2i oldSpritePosition, sf::Vector2i newSpritePosition);
  bool isOutOfBounds(sf::Vector2i spritePosition);
  sf::Vector2i gridPositionToFirstPixelPosition(sf::Vector2i gridPosition);
  void resetGridSystem();
private:
  std::vector<std::vector<std::vector<int>>> mGrid;
  sf::Vector2i positionToGridPosition(sf::Vector2i position);
  int mGridBoxSize = 32;
  int mGameSize = 1024;
  int mPixelsPerGridBox = mGameSize / mGridBoxSize;
};
