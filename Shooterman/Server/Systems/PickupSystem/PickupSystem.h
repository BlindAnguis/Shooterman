#pragma once

#include "../../Components/ComponentManager.h"
#include "../../Components/Components.h"
#include "../../../Common/Trace.h"
#include "../EntityCreatorSystem/EntityCreatorSystem.h"
#include "../ManaSystem/ManaSystem.h"

class PickupSystem : public Trace
{
public:
  PickupSystem(std::shared_ptr<MessageHandler> messageHandler, std::shared_ptr<EntityCreatorSystem> entityCreatorSystem, std::shared_ptr<ManaSystem> manaSystem);
  ~PickupSystem();
  void update();
private:
  ComponentManager<PickupComponent>* mPickupComponentManager;
  int mTimeToNextPickup;
  sf::Clock mPickupClock;
  std::shared_ptr<EntityCreatorSystem> mEntityCreatorSystem;
  std::shared_ptr<ManaSystem> mManaSystem;
};
