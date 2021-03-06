#pragma once
#include <map>
#include <stack>
#include "../Components/Components.h"
#include "../Components/ComponentManager.h"
#include "../../Common/Trace.h"

#define MAX_NUMBER_OF_ENTITIES 2000
struct Entity {
  int id;
};

class EntityManager : Trace {
public:
  EntityManager();
  ~EntityManager();
  static EntityManager& get();
  Entity* createEntity();
  void destroyEntity(int entityId);
  std::map<int, Entity*> getAllEntities();

private:
  int mLatestEntityId;
  std::map<int, Entity*> mEntities;
  std::stack<int> mFreeIds;
};

