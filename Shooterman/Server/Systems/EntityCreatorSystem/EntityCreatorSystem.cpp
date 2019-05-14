#include "EntityCreatorSystem.h"

EntityCreatorSystem::EntityCreatorSystem() :
  mEntityCreator(EntityCreator())
{
}

EntityCreatorSystem::~EntityCreatorSystem()
{
}

EntityCreatorSystem& EntityCreatorSystem::get() {
  static EntityCreatorSystem instance;
  return instance;
}

void EntityCreatorSystem::update() {
  auto it = mEntitiesToCreate.begin();
  while (it != mEntitiesToCreate.end()) {
    if (it->framesToCreation == 0) {
      createEntity(it->type, it->pos);
      it = mEntitiesToCreate.erase(it);
    } else {
      it->framesToCreation--;
      ++it;
    }
  }
}

void EntityCreatorSystem::addEntityToCreate(EntityType type, int framesToCreation) {
  mEntitiesToCreate.emplace_back(EntityToCreate{ type, sf::Vector2f(0, 0), framesToCreation });
}

void EntityCreatorSystem::addEntityToCreate(EntityType type, sf::Vector2f pos, int framesToCreation) {
  mEntitiesToCreate.emplace_back(EntityToCreate{ type, pos, framesToCreation });
}

Entity* EntityCreatorSystem::createEntity(EntityType type) {
  return createEntity(type, sf::Vector2f(0, 0));
}

Entity* EntityCreatorSystem::createEntity(EntityType type, sf::Vector2f pos) {
  switch (type)
  {
  case EntityType::PlayerMage:
    return mEntityCreator.createPlayer(PlayerClass::Mage, pos);
  case EntityType::PlayerKnight:
    return mEntityCreator.createPlayer(PlayerClass::Knight, pos);
  case EntityType::PlayerSpearman:
    return mEntityCreator.createPlayer(PlayerClass::Spearman, pos);
  case EntityType::Bullet:
    break;
  case EntityType::LightningBolt:
    break;
  case EntityType::LightningStrike:
    return mEntityCreator.createLightningStrike(pos);
  case EntityType::RandomPickup:
    return mEntityCreator.createRandomPickup();
  default:
    break;
  }

  return nullptr;
}