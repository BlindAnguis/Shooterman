#pragma once

#include "../../Components/ComponentManager.h"
#include "../../Components/Components.h"
#include "../../../Common/Trace.h"
#include "../../../Common/MessageHandler/MessageHandler.h"
#include "../../../Common//MessageHandler/Subscriber.h"
#include "../../../Common/MessageHandler/Interface.h"

class HealthSystem : Trace
{
public:
  HealthSystem(std::shared_ptr<MessageHandler> messageHandler);
  ~HealthSystem();
  void update();
  void changeHealth(int entityId, int addedHealthEffect, int damagerEntityId);
private:
  ComponentManager<HealthComponent>* mHealthComponentManager;
  ComponentManager<HealthChangerComponent>* mHealthChangerComponentManager;
  ComponentManager<CollisionComponent>* mCollisionComponentManager;
  Subscriber mSoundSubscriber;
  Interface mDeathInterface;
  std::shared_ptr<MessageHandler> mMessageHandler;
};
