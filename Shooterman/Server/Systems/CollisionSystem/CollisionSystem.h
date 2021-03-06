#pragma once

#include <memory>

#include "Collision.h"
#include "../../Components/ComponentManager.h"
#include "../../Components/Components.h"
#include "../GridSystem/GridSystem.h"
#include "../DeleteSystem/DeleteSystem.h"
#include "../../../Common/Trace.h"
#include "../../../Common/MessageHandler/MessageHandler.h"

class CollisionSystem : Trace
{
public:
  CollisionSystem(std::shared_ptr<MessageHandler> messageHandler, std::shared_ptr<DeleteSystem> deleteSystem);
  ~CollisionSystem();
  void handleAnyCollision(int causingColliderEntityId, float newXPos, float newYPos, std::shared_ptr<GridSystem> gridSystem);
  void resetCollisionInformation();
  void resetSystem();
private:
  void handleCollision(int causingColliderEntityId, std::shared_ptr<RenderComponent> causingColliderMovingComponent, int affectedCollideeEntityId, std::shared_ptr<RenderComponent> affectedCollideeMovingComponent);
  ComponentManager<RenderComponent>* mRenderComponentManager;
  ComponentManager<VelocityComponent>* mVelocityComponentManager;
  ComponentManager<CollisionComponent>* mCollisionComponentManager;
  std::shared_ptr<DeleteSystem> mDeleteSystem;
  std::shared_ptr<MessageHandler> mMessageHandler;
  bool mDebugMenuSubscribed = false;
  Subscriber mDebugMenuSubscriber;
  std::map<int, int> mCollisions; // First is the causing collider entity and second is the affected colidee entity.

  void handleDebugMessage();
};

