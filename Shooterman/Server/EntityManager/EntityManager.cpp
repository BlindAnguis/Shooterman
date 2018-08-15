#include "EntityManager.h"

EntityManager::EntityManager() : mLatestEntityId(0)
{
}

EntityManager::~EntityManager()
{
}

Entity* EntityManager::createEntity() {
  if (mLatestEntityId >= MAX_NUMBER_OF_ENTITIES) {
    // TODO: Log that max number of entities have been created.
    return new Entity{ 0 };
    
  }
  Entity* e = new Entity{ ++mLatestEntityId };
  mEntities.push_back(e);
  return e;
}

std::vector<Entity*> EntityManager::getAllEntities() {
  return mEntities;
}
