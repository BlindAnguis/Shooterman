#pragma once

#include <vector>
#include "../../EntityManager/EntityManager.h"
#include "../../EntityCreator/EntityCreator.h"

enum class EntityType {
  PlayerMage,
  PlayerKnight,
  PlayerSpearman,
  Bullet,
  LightningBolt,
  LightningStrike,
  RandomPickup,
};

struct EntityToCreate {
  EntityType type;
  sf::Vector2f pos;
  int framesToCreation;
  std::set<int> immuneEntityIds;
};

class EntityCreatorSystem : Trace
{
public:
  EntityCreatorSystem();
  ~EntityCreatorSystem();
  static EntityCreatorSystem& get();
  void update();
  void addEntityToCreate(EntityType type, sf::Vector2f pos, int framesToCreation, std::set<int> immuneEntityIds);
  void reset();
  Entity* createEntity(EntityType type, sf::Vector2f pos, std::set<int> immuneEntityIds);
private:
  std::vector<EntityToCreate> mEntitiesToCreate;
  EntityCreator mEntityCreator;
};
