#pragma once

#include <vector>
#include "../../EntityManager/EntityManager.h"
#include "../../EntityCreator/EntityCreator.h"

enum class EntityType {
  PlayerMage,
  PlayerKnight,
  PlayerSpearman,
  PlayerArcher,
  Bullet,
  LightningBolt,
  LightningStrike,
  RandomPickup,
};

struct EntityToCreate {
  int entityId;
  EntityType type;
  sf::Vector2f pos;
  int framesToCreation;
  std::set<int> immuneEntityIds;
};

class EntityCreatorSystem : Trace
{
public:
  EntityCreatorSystem(std::shared_ptr<MessageHandler> messageHandler, std::shared_ptr<DeleteSystem> deleteSystem, std::shared_ptr<GridSystem> gridSystem, std::shared_ptr<ManaSystem> manaSystem);
  ~EntityCreatorSystem();
  void update();
  void addEntityToCreate(int entityId, EntityType type, sf::Vector2f pos, int framesToCreation, std::set<int> immuneEntityIds);
  Entity* createEntity(int entityId, EntityType type, sf::Vector2f pos, std::set<int> immuneEntityIds);
private:
  std::vector<EntityToCreate> mEntitiesToCreate;
  EntityCreator* mEntityCreator;
  std::shared_ptr<MessageHandler> mMessageHandler;
};
