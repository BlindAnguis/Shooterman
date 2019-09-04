#include "DeleteSystem.h"

DeleteSystem::DeleteSystem() { mName = "SERVER: DELETE_SYSTEM"; }

DeleteSystem::~DeleteSystem() { TRACE_DEBUG1("Enter Destructor"); }

void DeleteSystem::addEntity(int entityId) {
  mEntitiesToRemove.emplace_back(entityId);
}

std::vector<int> DeleteSystem::getEntities() {
  auto tmp = mEntitiesToRemove;
  mEntitiesToRemove.clear();
  return tmp;
}