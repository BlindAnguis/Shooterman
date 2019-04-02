#pragma once

#include "../../Components/ComponentManager.h"
#include "../../Components/Components.h"
#include "../../../Common/Trace.h"

class HealthSystem : Trace
{
public:
  HealthSystem();
  ~HealthSystem();
  void update();
  static HealthSystem& get();
  void changeHealth(int entityId, int addedHealthEffect);
private:
  ComponentManager<HealthComponent>* mHealthComponentManager;
  ComponentManager<DamageComponent>* mDamageComponentManager;
  ComponentManager<CollisionComponent>* mCollisionComponentManager;
};
