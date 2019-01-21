#include "GridSystem.h"
#include <iostream>

GridSystem::GridSystem()
{
  for (int i = 0; i < mGridSize; i++) {
    mGrid.emplace_back(std::vector<std::vector<int>>());
  }

  for (int i = 0; i < mGridSize; i++) {
    for (int j = 0; j < mGridSize; j++) {
      mGrid.at(i).emplace_back(std::vector<int>());
    }
  }
}

GridSystem::~GridSystem()
{
}

std::vector<int> GridSystem::getNearEntities(sf::Vector2i spritePosition) {
  std::vector<int> nearEntities;
  auto nearGridPositions = getNearGridPositions(spritePosition);
  for (auto gridPosition : nearGridPositions) {
    for (auto entityId : mGrid.at(gridPosition.x).at(gridPosition.y)) {
      nearEntities.emplace_back(entityId);
    }
  }

  return nearEntities;
}

std::vector<sf::Vector2i> GridSystem::getNearGridPositions(sf::Vector2i spritePosition) {
  std::vector<sf::Vector2i> nearGridPositions;

  // Own position
  sf::Vector2i currentGridPos = positionToGridPosition(spritePosition);
  nearGridPositions.emplace_back(currentGridPos);

  // Near positions depending on where on the grid "own" position is.
  if (currentGridPos.x > 0 && currentGridPos.x < mGridSize - 1 && currentGridPos.y > 0 && currentGridPos.y < mGridSize - 1) {
    // In the middle
    //std::cout << "Middle" << std::endl;
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x - 1, currentGridPos.y - 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x, currentGridPos.y - 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x + 1, currentGridPos.y - 1));

    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x - 1, currentGridPos.y));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x + 1, currentGridPos.y));

    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x, currentGridPos.y + 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x - 1, currentGridPos.y + 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x + 1, currentGridPos.y + 1));
  }
  else if (currentGridPos.x == 0 && (currentGridPos.y > 0 && currentGridPos.y < mGridSize - 1)) {
    // Left vertical wall
    //std::cout << "Left Vertical Wall" << std::endl;
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x, currentGridPos.y - 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x + 1, currentGridPos.y - 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x + 1, currentGridPos.y));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x, currentGridPos.y + 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x + 1, currentGridPos.y + 1));
  }
  else if (currentGridPos.x == mGridSize - 1 && (currentGridPos.y > 0 && currentGridPos.y < mGridSize - 1)) {
    // Right vertical wall
    //std::cout << "Right Vertical Wall" << std::endl;
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x, currentGridPos.y - 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x - 1, currentGridPos.y - 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x - 1, currentGridPos.y));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x, currentGridPos.y + 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x - 1, currentGridPos.y + 1));
  }
  else if (currentGridPos.y == 0 && (currentGridPos.x > 0 && currentGridPos.x < mGridSize - 1)) {
    // Top horizontal wall
    //std::cout << "Top Horizontal Wall" << std::endl;
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x + 1, currentGridPos.y));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x + 1, currentGridPos.y + 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x, currentGridPos.y + 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x - 1, currentGridPos.y + 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x - 1, currentGridPos.y));
  }
  else if (currentGridPos.y == mGridSize - 1 && (currentGridPos.x > 0 && currentGridPos.x < mGridSize - 1)) {
    // Bottom horizontal wall
    //std::cout << "Bottom Horizontal Wall" << std::endl;
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x - 1, currentGridPos.y));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x - 1, currentGridPos.y - 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x, currentGridPos.y - 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x + 1, currentGridPos.y - 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x + 1, currentGridPos.y));
  }
  else if (currentGridPos.x == 0 && currentGridPos.y == 0) {
    // Top left corner
    //std::cout << "Top Left Corner" << std::endl;
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x + 1, currentGridPos.y));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x + 1, currentGridPos.y + 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x, currentGridPos.y + 1));
  }
  else if (currentGridPos.x == mGridSize - 1 && currentGridPos.y == 0) {
    // Top right corner
    //std::cout << "Top Right Corner" << std::endl;
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x - 1, currentGridPos.y));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x - 1, currentGridPos.y + 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x, currentGridPos.y + 1));
  }
  else if (currentGridPos.x == 0 && currentGridPos.y == mGridSize - 1) {
    // Bottom left corner
    //std::cout << "Bottom Left Corner" << std::endl;
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x, currentGridPos.y - 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x + 1, currentGridPos.y - 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x + 1, currentGridPos.y));
  }
  else if (currentGridPos.x == mGridSize - 1 && currentGridPos.y == mGridSize - 1) {
    // Bottom right corner
    //std::cout << "Bottom Right Corner" << std::endl;
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x - 1, currentGridPos.y));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x - 1, currentGridPos.y - 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x, currentGridPos.y - 1));
  }

  return nearGridPositions;
}

sf::Vector2i GridSystem::positionToGridPosition(sf::Vector2i position)
{
  auto pos = sf::Vector2i(position.x / mPixelsPerGridBox, position.y / mPixelsPerGridBox);
  if (pos.x < 0) {
    pos.x = 0;
  }
  if (pos.y < 0) {
    pos.y = 0;
  }
  if (pos.x >= mGridSize) {
    pos.x = mGridSize - 1;
  }
  if (pos.y >= mGridSize) {
    pos.y = mGridSize - 1;
  }

  return pos;
}

void GridSystem::addEntity(int entityId, sf::Vector2i spritePosition) {
  auto gridPos = positionToGridPosition(spritePosition);
  auto entities = &mGrid.at(gridPos.x).at(gridPos.y);
  if (std::find(std::begin(*entities), std::end(*entities), entityId) == std::end(*entities)) {
    entities->emplace_back(entityId);
  }
}

void GridSystem::removeEntity(int entityId, sf::Vector2i spritePosition) {
  auto gridPos = positionToGridPosition(spritePosition);
  auto entities = &mGrid.at(gridPos.x).at(gridPos.y);
  entities->erase(std::remove(std::begin(*entities), std::end(*entities), entityId), std::end(*entities));
}

void GridSystem::moveEntity(int entityId, sf::Vector2i oldSpritePosition, sf::Vector2i newSpritePosition)
{
  removeEntity(entityId, oldSpritePosition);
  addEntity(entityId, newSpritePosition);
}

bool GridSystem::isOutOfBounds(sf::Vector2i spritePosition) {
  if (spritePosition.x < 0 || spritePosition.x > mGameSize || spritePosition.y < 0 || spritePosition.y > mGameSize) {
    return true;
  }
  return false;
}
