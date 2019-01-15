#include "EntityManager.h"

EntityManager::EntityManager() : mLatestEntityId(0) {
  mName = "SERVER: ENTITY_MANAAGER";
  for (int i = 0; i < MAX_NUMBER_OF_ENTITIES; ++i) {
    mFreeIds.push(i);
  }
}

EntityManager::~EntityManager() {
}

Entity* EntityManager::createEntity() {
  if (mFreeIds.empty()) {
    TRACE_ERROR("MAXIMUN NUMBER OF ENTITIES REACHED!");
    return new Entity{ 0 };
  }

  int newId = mFreeIds.top();
  mFreeIds.pop();
  Entity* e = new Entity{ newId };
  mEntities.emplace(newId, e);
  return e;
}

void EntityManager::destroyEntity(int entityId) {
  auto it = mEntities.find(entityId);
  if (it != mEntities.end()) {
    // Remove all related componentes
    delete it->second;
    mEntities.erase(entityId);
    mFreeIds.push(entityId);
  }
}