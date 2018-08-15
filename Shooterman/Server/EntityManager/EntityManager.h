#pragma once
#include <vector>
#include <map>
#include "../Components/Components.h"
#include "../Components/ComponentManager.h"

#define MAX_NUMBER_OF_ENTITIES 128
struct Entity {
  int id;
};

class EntityManager
{
public:
  EntityManager();
  ~EntityManager();
  Entity* createEntity();
  std::vector<Entity*> getAllEntities();
private:
  int mLatestEntityId;
  std::vector<Entity*> mEntities;
};

