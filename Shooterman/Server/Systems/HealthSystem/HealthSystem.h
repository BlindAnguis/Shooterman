#pragma once

#include "../../Components/ComponentManager.h"
#include "../../Components/Components.h"
#include "../../../Common/Trace.h"
#include "../../../Common/MessageHandler/MessageHandler.h"
#include "../../../Common//MessageHandler/Subscriber.h"

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
  ComponentManager<HealthChangerComponent>* mHealthChangerComponentManager;
  ComponentManager<CollisionComponent>* mCollisionComponentManager;
  Subscriber mSoundSubscriber;
};
