#pragma once

#include "../../Components/ComponentManager.h"
#include "../../Components/Components.h"
#include "../../../Common/Trace.h"
#include "../../EntityCreator/EntityCreator.h"

class PickupSystem : public Trace
{
public:
  PickupSystem(EntityCreator* entityCreator);
  ~PickupSystem();
  void update();
private:
  ComponentManager<PickupComponent>* mPickupComponentManager;
  int mTimeToNextPickup;
  sf::Clock mPickupClock;
  EntityCreator* mEntityCreator;
};
