#pragma once
#include <map>
#include <stack>
#include "../Components/Components.h"
#include "../Components/ComponentManager.h"
#include "../../Common/Trace.h"

#define MAX_NUMBER_OF_ENTITIES 512
struct Entity {
  int id;
};

class EntityManager : Trace {
public:
  EntityManager();
  ~EntityManager();
  Entity* createEntity();
  void destroyEntity(int entityId);

private:
  int mLatestEntityId;
  std::map<int, Entity*> mEntities;
  std::stack<int> mFreeIds;
};

