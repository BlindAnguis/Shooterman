#pragma once

#include "../../Components/ComponentManager.h"
#include "../../Components/Components.h"
#include "../../../Common/Trace.h"

class ManaSystem : Trace
{
public:
  ManaSystem();
  ~ManaSystem();
  void update();
  void changeMana(int entityId, int addedManaEffect);
private:
  ComponentManager<ManaComponent>* mManaComponentManager;
};
