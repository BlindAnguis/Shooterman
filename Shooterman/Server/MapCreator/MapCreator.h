#pragma once
#include "../../Common/Trace.h"
#include "../Components/Components.h"
#include "../Components/ComponentManager.h"
#include "../EntityManager/EntityManager.h"
#include "../Systems/GridSystem/GridSystem.h"

#include <array>

class MapCreator : public Trace {
public:
  MapCreator(EntityManager *entityManager, GridSystem *gridSystem);
  ~MapCreator();

  void createMap(std::array<std::array<Textures, 32>, 32> gameMap);
  std::vector<std::pair<float, float>> getSpawnPositions();

private:
  EntityManager *mEntityManager;
  ComponentManager<RenderComponent>* mRenderComponentManager;
  ComponentManager<CollisionComponent>* mCollisionComponentManager;
  ComponentManager<HealthComponent>* mHealthComponentManager;
  ComponentManager<HealthChangerComponent>* mHealthChangerComponentManager;

  GridSystem *mGridSystem;

  std::array<sf::Texture*, 99> mTextures;

  std::vector<std::pair<float, float>> mSpawnPositions;

  Entity* createHorizontalWall(float xPos, float yPos);
  Entity* createVerticalWall(float xPos, float yPos);
  Entity* createFloor(Textures texture, sf::Vector2f position, sf::IntRect positionInTexture);
  Entity* createFloorSpikes(sf::Vector2f position);
  sf::Texture* loadTexture(std::string fileName);
};

