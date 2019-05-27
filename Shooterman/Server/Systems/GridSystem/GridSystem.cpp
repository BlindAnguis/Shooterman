#include "GridSystem.h"

GridSystem::GridSystem()
{
  for (int i = 0; i < mGridBoxSize; i++) {
    mGrid.emplace_back(std::vector<std::vector<int>>());
  }

  for (int i = 0; i < mGridBoxSize; i++) {
    for (int j = 0; j < mGridBoxSize; j++) {
      mGrid.at(i).emplace_back(std::vector<int>());
    }
  }

  mName = "SERVER: GRID_SYSTEM";
}

GridSystem::~GridSystem()
{
  TRACE_DEBUG("Enter Destructor");
}

GridSystem & GridSystem::get()
{
  static GridSystem instance;
  return instance;
}

void GridSystem::resetGridSystem() {
  mGrid.clear();
  for (int i = 0; i < mGridBoxSize; i++) {
    mGrid.emplace_back(std::vector<std::vector<int>>());
  }

  for (int i = 0; i < mGridBoxSize; i++) {
    for (int j = 0; j < mGridBoxSize; j++) {
      mGrid.at(i).emplace_back(std::vector<int>());
    }
  }
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
  if (currentGridPos.x > 0 && currentGridPos.x < mGridBoxSize - 1 && currentGridPos.y > 0 && currentGridPos.y < mGridBoxSize - 1) {
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
  else if (currentGridPos.x == 0 && (currentGridPos.y > 0 && currentGridPos.y < mGridBoxSize - 1)) {
    // Left vertical wall
    //std::cout << "Left Vertical Wall" << std::endl;
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x, currentGridPos.y - 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x + 1, currentGridPos.y - 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x + 1, currentGridPos.y));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x, currentGridPos.y + 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x + 1, currentGridPos.y + 1));
  }
  else if (currentGridPos.x == mGridBoxSize - 1 && (currentGridPos.y > 0 && currentGridPos.y < mGridBoxSize - 1)) {
    // Right vertical wall
    //std::cout << "Right Vertical Wall" << std::endl;
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x, currentGridPos.y - 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x - 1, currentGridPos.y - 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x - 1, currentGridPos.y));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x, currentGridPos.y + 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x - 1, currentGridPos.y + 1));
  }
  else if (currentGridPos.y == 0 && (currentGridPos.x > 0 && currentGridPos.x < mGridBoxSize - 1)) {
    // Top horizontal wall
    //std::cout << "Top Horizontal Wall" << std::endl;
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x + 1, currentGridPos.y));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x + 1, currentGridPos.y + 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x, currentGridPos.y + 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x - 1, currentGridPos.y + 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x - 1, currentGridPos.y));
  }
  else if (currentGridPos.y == mGridBoxSize - 1 && (currentGridPos.x > 0 && currentGridPos.x < mGridBoxSize - 1)) {
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
  else if (currentGridPos.x == mGridBoxSize - 1 && currentGridPos.y == 0) {
    // Top right corner
    //std::cout << "Top Right Corner" << std::endl;
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x - 1, currentGridPos.y));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x - 1, currentGridPos.y + 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x, currentGridPos.y + 1));
  }
  else if (currentGridPos.x == 0 && currentGridPos.y == mGridBoxSize - 1) {
    // Bottom left corner
    //std::cout << "Bottom Left Corner" << std::endl;
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x, currentGridPos.y - 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x + 1, currentGridPos.y - 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x + 1, currentGridPos.y));
  }
  else if (currentGridPos.x == mGridBoxSize - 1 && currentGridPos.y == mGridBoxSize - 1) {
    // Bottom right corner
    //std::cout << "Bottom Right Corner" << std::endl;
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x - 1, currentGridPos.y));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x - 1, currentGridPos.y - 1));
    nearGridPositions.emplace_back(sf::Vector2i(currentGridPos.x, currentGridPos.y - 1));
  }

  return nearGridPositions;
}

std::vector<sf::Vector2i> GridSystem::getEmptyGridPositions() {
  std::vector<sf::Vector2i> emptyGridPositions;
  for (int x = 0; x < mGridBoxSize; x++) {
    for (int y = 0; y < mGridBoxSize; y++) {
      if (mGrid.at(x).at(y).empty()) {
        emptyGridPositions.emplace_back(sf::Vector2i(x, y));
      }
    }
  }
  return emptyGridPositions;
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
  if (pos.x >= mGridBoxSize) {
    pos.x = mGridBoxSize - 1;
  }
  if (pos.y >= mGridBoxSize) {
    pos.y = mGridBoxSize - 1;
  }

  return pos;
}

sf::Vector2i GridSystem::gridPositionToFirstPixelPosition(sf::Vector2i position)
{
  return sf::Vector2i(position.x * mPixelsPerGridBox, position.y * mPixelsPerGridBox);
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
