#include "DeleteSystem.h"

DeleteSystem::DeleteSystem() {}

DeleteSystem::~DeleteSystem() {}

DeleteSystem& DeleteSystem::get() {
  static DeleteSystem instance;
  return instance;
}

void DeleteSystem::addEntity(int entityId) {
  mEntitiesToRemove.emplace_back(entityId);
}

std::vector<int> DeleteSystem::getEntities() {
  auto tmp = mEntitiesToRemove;
  mEntitiesToRemove.clear();
  return tmp;
}