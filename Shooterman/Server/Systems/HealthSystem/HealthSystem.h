#pragma once

#include "../../Components/ComponentManager.h"
#include "../../Components/Components.h"
#include "../../../Common/Trace.h"
#include "../../../Common/MessageHandler/MessageHandler.h"
#include "../../../Common//MessageHandler/Subscriber.h"

class HealthSystem : Trace
{
public:
  HealthSystem(std::shared_ptr<MessageHandler> messageHandler);
  ~HealthSystem();
  void update();
  static HealthSystem& get(std::shared_ptr<MessageHandler> messageHandler);
  void changeHealth(int entityId, int addedHealthEffect);
private:
  ComponentManager<HealthComponent>* mHealthComponentManager;
  ComponentManager<HealthChangerComponent>* mHealthChangerComponentManager;
  ComponentManager<CollisionComponent>* mCollisionComponentManager;
  Subscriber mSoundSubscriber;
  std::shared_ptr<MessageHandler> mMessageHandler;
};
