#include "EntityCreatorSystem.h"

EntityCreatorSystem::EntityCreatorSystem(std::shared_ptr<MessageHandler> messageHandler, std::shared_ptr<DeleteSystem> deleteSystem, std::shared_ptr<GridSystem> gridSystem, std::shared_ptr<ManaSystem> manaSystem) :
  mEntityCreator(new EntityCreator(messageHandler, deleteSystem, this, gridSystem, manaSystem)),
  mMessageHandler(messageHandler)
{
  mName = "SERVER: ENTITY_CREATOR_SYSTEM";
}

EntityCreatorSystem::~EntityCreatorSystem()
{
  TRACE_DEBUG1("Enter Destructor");
  if (mEntityCreator != nullptr)
    delete(mEntityCreator);
}

void EntityCreatorSystem::update() {
  auto it = mEntitiesToCreate.begin();
  while (it != mEntitiesToCreate.end()) {
    if (it->framesToCreation == 0) {
      createEntity(it->type, it->pos, it->immuneEntityIds);
      it = mEntitiesToCreate.erase(it);
    } else {
      it->framesToCreation--;
      ++it;
    }
  }
}

void EntityCreatorSystem::addEntityToCreate(EntityType type, sf::Vector2f pos, int framesToCreation, std::set<int> immuneEntityIds) {
  mEntitiesToCreate.emplace_back(EntityToCreate{ type, pos, framesToCreation, immuneEntityIds });
}

Entity* EntityCreatorSystem::createEntity(EntityType type, sf::Vector2f pos, std::set<int> immuneEntityIds) {
  switch (type)
  {
  case EntityType::PlayerMage:
    return mEntityCreator->createPlayer(PlayerClass::Mage, pos);
  case EntityType::PlayerKnight:
    return mEntityCreator->createPlayer(PlayerClass::Knight, pos);
  case EntityType::PlayerSpearman:
    return mEntityCreator->createPlayer(PlayerClass::Spearman, pos);
  case EntityType::PlayerArcher:
    return mEntityCreator->createPlayer(PlayerClass::Archer, pos);
  case EntityType::Bullet:
    break;
  case EntityType::LightningBolt:
    break;
  case EntityType::LightningStrike:
    return mEntityCreator->createLightningStrike(pos, immuneEntityIds);
  case EntityType::RandomPickup:
    return mEntityCreator->createRandomPickup();
  default:
    break;
  }

  return nullptr;
}
