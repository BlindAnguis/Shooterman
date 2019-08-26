#pragma once

#include "../../Components/ComponentManager.h"
#include "../../Components/Components.h"
#include "../../../Common/Trace.h"
#include "../EntityCreatorSystem/EntityCreatorSystem.h"

class PickupSystem : public Trace
{
public:
  PickupSystem(std::shared_ptr<MessageHandler> messageHandler);
  ~PickupSystem();
  static PickupSystem& get(std::shared_ptr<MessageHandler> messageHandler);
  void update();
private:
  ComponentManager<PickupComponent>* mPickupComponentManager;
  int mTimeToNextPickup;
  sf::Clock mPickupClock;
  EntityCreatorSystem* mEntityCreatorSystem;
};
